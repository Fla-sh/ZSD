import pandas
import matplotlib.pyplot as mpl;


def main():
    with open("times_zsd.csv") as file:
        data = pandas.read_csv(file)
        # print(data)
        mpl.figure(figsize=(10,10))
        ax = mpl.gca()
        operations = ["insert", "remove", "remove_list"]
        for op in operations:
            mask = data[data.operation == op]
            mask.plot(kind="line", ax=ax, x="size", y="time", logx=True);
        mpl.title("Lista")
        mpl.legend(operations)
        mpl.savefig("list", dpi=1200)


if __name__ == "__main__":
    main()