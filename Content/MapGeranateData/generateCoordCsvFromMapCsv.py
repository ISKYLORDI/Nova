import csv
from readMapCsv import read_map_csv


def write_data(
    data: list[list[dict[int, int]]],
    writer: csv.DictWriter,
    count: int,
    y_index: int,
    x_index: int,
) -> int:
    value_dict = data[y_index][x_index]
    for z_value in value_dict.keys():
        # floor
        if value_dict[z_value] == 0:
            writer.writerow(
                {
                    "---": count,
                    "x": x_index * 100,
                    "y": y_index * 100,
                    "z": z_value * 100,
                    "type": 0,
                }
            )
            count += 1
            writer.writerow(
                {
                    "---": count,
                    "x": x_index * 100,
                    "y": y_index * 100,
                    "z": z_value * 100 - 50,
                    "type": 1,
                }
            )
            count += 1
        # obstacle
        if value_dict[z_value] == 1:
            writer.writerow(
                {
                    "---": count,
                    "x": x_index * 100,
                    "y": y_index * 100,
                    "z": z_value * 100 - 50,
                    "type": 1,
                }
            )

            count += 1

    return count


def save_csv(file_path: str, data: list[list[dict[int, int]]]) -> None:
    with open(file_path, "w", newline="") as f:
        filednames = ["---", "x", "y", "z", "type"]
        writer = csv.DictWriter(f, fieldnames=filednames)

        writer.writeheader()
        count = 1
        for y_index in range(len(data)):
            for x_index in range(len(data[y_index])):
                count = write_data(data, writer, count, y_index, x_index)


# data = [
#     [0, 1, 0],
#     [1, 0, 1],
#     [0, 1, 0],
# ]

data = read_map_csv("edit_me/Map2.csv")

save_csv("out/MapCoordData.csv", data)
