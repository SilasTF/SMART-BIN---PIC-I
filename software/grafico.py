"""
grafico.py - Smart Bin
Le medicoes.csv (gerado pelo logger.py) e plota o nivel da lixeira.

Instalar:  pip install pandas matplotlib
Usar:      python grafico.py
           python grafico.py outro_arquivo.csv

Ajuste ALTURA_VAZIA_CM e ALTURA_CHEIA_CM para a sua lixeira real.
"""

import sys

import matplotlib.pyplot as plt
import pandas as pd

ARQUIVO = sys.argv[1] if len(sys.argv) > 1 else "medicoes.csv"
ALTURA_VAZIA_CM = 30.0   # distancia medida com a lixeira vazia
ALTURA_CHEIA_CM = 5.0    # distancia medida com a lixeira cheia


def main():
    try:
        df = pd.read_csv(ARQUIVO, parse_dates=["hora"])
    except FileNotFoundError:
        print(f"Arquivo {ARQUIVO} nao encontrado. Rode o logger.py primeiro.")
        sys.exit(1)

    if df.empty:
        print("O CSV esta vazio.")
        sys.exit(1)

    # converte distancia em percentual de enchimento (0 a 100)
    df["nivel_pct"] = (
        (ALTURA_VAZIA_CM - df["distancia_cm"])
        / (ALTURA_VAZIA_CM - ALTURA_CHEIA_CM)
        * 100
    ).clip(0, 100)

    fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(10, 7), sharex=True)

    ax1.plot(df["hora"], df["distancia_cm"], color="tab:blue")
    ax1.set_ylabel("Distancia (cm)")
    ax1.set_title("Smart Bin - leituras do sensor")
    ax1.grid(True, alpha=0.3)

    ax2.plot(df["hora"], df["nivel_pct"], color="tab:green")
    ax2.axhline(80, color="red", linestyle="--", label="Alerta (80%)")
    ax2.set_ylabel("Nivel de enchimento (%)")
    ax2.set_xlabel("Hora")
    ax2.set_ylim(0, 105)
    ax2.legend()
    ax2.grid(True, alpha=0.3)

    fig.autofmt_xdate()
    fig.tight_layout()
    fig.savefig("grafico_nivel.png", dpi=150)
    print("Grafico salvo em grafico_nivel.png")
    plt.show()


if __name__ == "__main__":
    main()
