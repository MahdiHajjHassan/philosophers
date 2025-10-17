#!/bin/bash
set -e

# Configuration
PROJECT=big-data-etl-473708
REGION=europe-west1
BUCKET=big-data-etl-473708-raw-data-csv
WF=df-pageviews-daily

echo "=========================================="
echo "Pipeline Verification and Scheduler Update"
echo "=========================================="
echo ""

# Sanity Check 1: Volume by date/hour (last 3 days)
echo "1️⃣  Checking volume by date/hour (last 3 days)..."
echo ""
bq query --use_legacy_sql=false '
SELECT event_date, event_hour, COUNT(*) AS rows
FROM `big-data-etl-473708.pageviews.hourly_raw`
WHERE event_date >= DATE_SUB(CURRENT_DATE(), INTERVAL 3 DAY)
GROUP BY event_date, event_hour
ORDER BY event_date DESC, event_hour DESC
LIMIT 48'

echo ""
echo "=========================================="
echo ""

# Sanity Check 2: Latest load hours
echo "2️⃣  Checking latest load hours..."
echo ""
bq query --use_legacy_sql=false '
SELECT TIMESTAMP_TRUNC(load_ts, HOUR) AS load_hour, COUNT(*) AS rows
FROM `big-data-etl-473708.pageviews.hourly_raw`
WHERE event_date >= DATE_SUB(CURRENT_DATE(), INTERVAL 2 DAY)
GROUP BY load_hour ORDER BY load_hour DESC LIMIT 12'

echo ""
echo "=========================================="
echo ""

# Sanity Check 3: Spot-check sample rows from latest day
echo "3️⃣  Spot-checking sample rows from the latest day..."
echo ""
bq query --use_legacy_sql=false '
WITH d AS (
  SELECT MAX(event_date) AS mx
  FROM `big-data-etl-473708.pageviews.hourly_raw`
)
SELECT * FROM `big-data-etl-473708.pageviews.hourly_raw`, d
WHERE event_date = d.mx
ORDER BY load_ts DESC
LIMIT 20'

echo ""
echo "=========================================="
echo ""

# Update Scheduler
echo "4️⃣  Updating Cloud Scheduler job to use pageviews-hourly-raw template..."
echo ""
gcloud scheduler jobs update http wf-pv-daily \
  --location=$REGION \
  --message-body="{
    \"argument\": \"{\\\"project\\\":\\\"$PROJECT\\\",\\\"region\\\":\\\"$REGION\\\",\\\"bucket\\\":\\\"$BUCKET\\\",\\\"dataset\\\":\\\"pageviews\\\",\\\"table\\\":\\\"hourly_raw\\\",\\\"template_gcs\\\":\\\"gs://$BUCKET/df/templates/pageviews-hourly-raw\\\",\\\"network\\\":\\\"df-vpc\\\",\\\"subnetwork\\\":\\\"df-subnet\\\",\\\"worker_sa\\\":\\\"\\\"}\"
  }"

echo ""
echo "✅ Scheduler job updated successfully!"
echo ""

# Resume the scheduler
echo "5️⃣  Resuming the daily trigger..."
echo ""
gcloud scheduler jobs resume wf-pv-daily --location=$REGION

echo ""
echo "✅ Scheduler job resumed!"
echo ""
echo "=========================================="
echo ""

# Manual run (optional)
read -p "Do you want to trigger a manual workflow run now? (y/n) " -n 1 -r
echo ""
if [[ $REPLY =~ ^[Yy]$ ]]
then
    echo "6️⃣  Running manual workflow execution..."
    echo ""
    gcloud workflows run $WF --location=$REGION \
      --data="{
        \"project\":\"$PROJECT\",
        \"region\":\"$REGION\",
        \"bucket\":\"$BUCKET\",
        \"dataset\":\"pageviews\",
        \"table\":\"hourly_raw\",
        \"template_gcs\":\"gs://$BUCKET/df/templates/pageviews-hourly-raw\",
        \"network\":\"df-vpc\",
        \"subnetwork\":\"df-subnet\",
        \"worker_sa\":\"\"
      }"
    echo ""
    echo "✅ Manual workflow triggered!"
fi

echo ""
echo "=========================================="
echo "✅ All checks and updates completed!"
echo "=========================================="
echo ""
echo "📝 Optional: Create firewall rule for Dataflow shuffle"
echo "   (only needed if running >1 worker without Dataflow Shuffle)"
echo ""
echo "   gcloud compute firewall-rules create dataflow-shuffle \\"
echo "     --network=df-vpc \\"
echo "     --allow=tcp:12345-12346 \\"
echo "     --target-tags=dataflow"
echo ""
