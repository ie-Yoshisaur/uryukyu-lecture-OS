import argparse
import pandas as pd
import matplotlib.pyplot as plt


def plot_bars(file_name):
    df = pd.read_csv(file_name)
    sec = df['sec']
    user = df['user']
    sys = df['sys']
    idle = df['idle']

    fig, ax = plt.subplots()
    ax.bar(sec, user, label='user')
    ax.bar(sec, sys, bottom=user, label='sys')
    ax.bar(sec, idle, bottom=user+sys, label='idle')

    ax.set_xlabel('sec')
    ax.set_ylabel('Usage %')
    ax.legend()
    plt.title(file_name)

    plt.show()


if __name__ == '__main__':
    parser = argparse.ArgumentParser()
    parser.add_argument('file_name', help='CSV file name')
    args = parser.parse_args()

    plot_bars(args.file_name)
