import csv
from readMapCsv import read_map_csv


def vertical_shift(
    data: list[list[dict[int, int]]], file_path: str, delta: int
) -> None:
    with open(file_path, "w", newline="") as f:
        filednames = ["---"] + [str(i + 1) for i in range(len(data[0]))]
        writer = csv.DictWriter(f, fieldnames=filednames)

        writer.writeheader()

        for y_index in range(len(data)):
            row_content = {"---": y_index + 1}
            for x_index in range(len(data[y_index])):
                value_dict = data[y_index][x_index]
                block_content = ""
                for z_value in value_dict.keys():
                    if value_dict[z_value] == 0:
                        block_content += "f"
                    elif value_dict[z_value] == 1:
                        block_content += "o"

                    block_content += str(z_value + delta)
                    block_content += ";"

                if len(block_content) != 0:
                    block_content = block_content[:-1]
                row_content[str(x_index + 1)] = block_content

            writer.writerow(row_content)


file = "edit_me/shifter.csv"
data = read_map_csv(file)
print
vertical_shift(data, file, -4)
