"""
logger.py - Smart Bin
Le o monitor serial do ESP32 (linhas como "Distancia: 12.3 cm")
e salva cada medicao em medicoes.csv com data e hora.

Instalar:  pip install pyserial
Usar:      python logger.py            (usa a porta definida em PORTA)
           python logger.py COM5       (ou passe a porta como argumento)
Parar:     Ctrl+C

IMPORTANTE: feche o Monitor Serial da Arduino IDE / PlatformIO antes,
senao a porta fica ocupada.
"""

import csv
import os
import re
import sys
from datetime import datetime

import serial
from serial.tools import list_ports

PORTA = "COM3"          # Linux: /dev/ttyUSB0  |  Mac: /dev/cu.usbserial-XXXX
BAUD = 115200
ARQUIVO = "medicoes.csv"


def listar_portas():
    portas = list(list_ports.comports())
    if not portas:
        print("Nenhuma porta serial encontrada. O ESP32 esta conectado?")
    else:
        print("Portas disponiveis:")
        for p in portas:
            print(f"  {p.device} - {p.description}")


def main():
    porta = sys.argv[1] if len(sys.argv) > 1 else PORTA
    novo = not os.path.exists(ARQUIVO)

    try:
        ser = serial.Serial(porta, BAUD, timeout=2)
    except serial.SerialException as e:
        print(f"Nao consegui abrir {porta}: {e}\n")
        listar_portas()
        sys.exit(1)

    print(f"Lendo {porta} a {BAUD} baud. Salvando em {ARQUIVO}. Ctrl+C para parar.")

    with ser, open(ARQUIVO, "a", newline="", encoding="utf-8") as f:
        w = csv.writer(f)
        if novo:
            w.writerow(["hora", "distancia_cm"])
        try:
            while True:
                linha = ser.readline().decode(errors="ignore").strip()
                if not linha:
                    continue
                m = re.search(r"(\d+(?:\.\d+)?)\s*cm", linha)
                if m:
                    valor = float(m.group(1))
                    w.writerow([datetime.now().isoformat(timespec="seconds"), valor])
                    f.flush()
                    print(f"{datetime.now():%H:%M:%S}  {valor} cm")
        except KeyboardInterrupt:
            print("\nEncerrado.")


if __name__ == "__main__":
    main()
