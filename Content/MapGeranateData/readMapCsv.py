import csv


def read_map_csv(file_path: str) -> list[list[dict[int, int]]]:
    # data -> [y_value][x_value][z value] = type

    data: list[list[int]] = []
    with open(file_path, "r") as f:
        reader = csv.DictReader(f)
        # headers = next(reader)
        y_value = 0
        for row in reader:
            data.append([])
            for index in range(len(row)):
                if index == 0:
                    continue

                content = {}
                value_list = row[str(index)].split(";")

                for value in value_list:
                    if value == "":
                        continue
                    if value[0] == "f":
                        content[int(value[1:])] = 0
                    elif value[0] == "o":
                        content[int(value[1:])] = 1

                data[y_value].append(content)

            y_value += 1

    return data


read_map_csv("edit_me/Map2.csv")
