import csv


def add_f(file_path: str, data: list[list[dict[int, int]]]) -> None:
    with open(file_path, "w", newline="") as f:
        filednames = ["---", "x", "y", "z", "type"]
        writer = csv.DictWriter(f, fieldnames=filednames)
        writer.writeheader()

        for y_index in range(len(data)):
            for x_index in range(len(data[y_index])):
                writer.writerow(
                    {
                        "---": count,
                        "x": x_index * 100,
                        "y": y_index * 100,
                        "z": z_value * 100 - 50,
                        "type": 1,
                    }
                )
