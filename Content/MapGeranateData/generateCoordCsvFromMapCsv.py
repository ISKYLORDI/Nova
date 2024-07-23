import csv


def read_map_csv(file_path: str) -> list[list[int]]:
    data: list[list[int]] = []
    with open(file_path, "r") as f:
        reader = csv.DictReader(f)
        headers = next(reader)
        y_value = 0
        for row in reader:
            data.append([])
            for z_value in row.values():
                data[y_value].append(int(z_value) if z_value != "" else None)
            data[y_value].pop(0)
            y_value += 1

    return data


def save_csv(file_path: str, data: list[list[int]]) -> None:
    with open(file_path, "w", newline="") as f:
        filednames = ["---", "x", "y", "z"]
        writer = csv.DictWriter(f, fieldnames=filednames)

        writer.writeheader()
        count = 1
        for y_index in range(len(data)):
            for x_index in range(len(data[y_index])):
                value = data[y_index][x_index]
                if value != None:
                    writer.writerow(
                        {
                            "---": count,
                            "x": x_index * 100,
                            "y": y_index * 100,
                            "z": value * 100,
                        }
                    )
                    count += 1


data = [
    [0, 1, 0],
    [1, 0, 1],
    [0, 1, 0],
]

data = read_map_csv("edit_me/Map1.csv")

save_csv("out/MapCoordData.csv", data)
