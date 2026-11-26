#!/usr/bin/env bash
cd "$(dirname "$0")"
source .venv/bin/activate
export TELEGRAM_TOKEN="troque-por-seu-token"
export TELEGRAM_CHAT_ID="troque-pelo-chat-it"
export API_KEY="troque-pela-api-key"
export DASHBOARD_PASSWORD="smartbin123" #senha padrao, troque se quiser
python app.py
