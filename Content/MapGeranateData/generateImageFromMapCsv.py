import csv
from PIL import Image, ImageDraw

from readMapCsv import read_map_csv


def save_img(file_path: str, data: list[list[int]]) -> None:
    grid_size = 30
    line_width = 2
    x_len = len(data[0])
    y_len = len(data)
    size = (
        x_len * (grid_size + line_width) + line_width,
        y_len * (grid_size + line_width) + line_width,
    )
    image = Image.new("RGB", size, (140, 140, 140))
    draw_img = ImageDraw.Draw(image)

    for x_index in range(x_len):
        for y_index in range(y_len):
            value = data[y_index][x_index]
            color = (200, 200, 200) if value != None else (20, 20, 20)
            draw_img.rectangle(
                (
                    x_index * (grid_size + line_width) + line_width,
                    y_index * (grid_size + line_width) + line_width,
                    (x_index + 1) * (grid_size + line_width) - line_width,
                    (y_index + 1) * (grid_size + line_width) - line_width,
                ),
                fill=color,
            )
            if value != None:
                draw_img.text(
                    (
                        x_index * (grid_size + line_width) + grid_size // 2 - 5,
                        y_index * (grid_size + line_width) + grid_size // 2 - 5,
                    ),
                    str(value),
                    fill=(0, 0, 0),
                )

    image.save(file_path)


data = read_map_csv("edit_me/Map2.csv")
save_img("out/test_image.png", data)
