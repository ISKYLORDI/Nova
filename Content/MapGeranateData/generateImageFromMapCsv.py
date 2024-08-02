import csv
from PIL import Image, ImageDraw, ImageFont

from readMapCsv import read_map_csv

ground_colors = [
    (11, 83, 69),
    # (14, 102, 85),
    # (17, 122, 101),
    # (19, 141, 117),
    # (22, 160, 133),
    # (69, 179, 157),
    (247, 220, 111),
    # (162, 217, 206),
]

# obt_colors = [
#     (125, 102, 8),
#     (154, 125, 10),
#     (183, 149, 11),
#     (212, 172, 13),
#     (241, 196, 15),
#     (244, 208, 63),
#     (247, 220, 111),
#     (249, 231, 159),
# ]

obt_colors = [(98, 101, 103), (202, 207, 210)]
# (121, 125, 127),
# (144, 148, 151),
# (166, 172, 175),
# (189, 195, 199),
# ()


void_color = (20, 30, 40)
# grid_color = (77, 86, 86)
grid_color = (33, 47, 60)


def color_lerp(data: list[tuple[int]], max: int, min: int, n: int) -> tuple[int]:

    # n在max和min之间的百分比大小
    n_weight = (n - min) / (max - min)
    if n_weight == 1:
        return data[-1]
    # data每个区间的百分比大小
    block_avg_weight = 1 / (len(data) - 1)
    # n所在的区间的index
    idx_of_block = int(n_weight / block_avg_weight)
    # n在的区间内占比的百分比
    n_block_weight = (n_weight - idx_of_block * block_avg_weight) / block_avg_weight

    return tuple(
        [
            round(
                (data[idx_of_block + 1][i] - data[idx_of_block][i]) * n_block_weight
                + data[idx_of_block][i]
            )
            for i in range(3)
        ]
    )


def get_max_min(data: list[list[dict[int, int]]]) -> tuple[int]:
    z_max = -9999
    z_min = 9999

    x_len = len(data[0])
    y_len = len(data)
    for x_index in range(x_len):
        for y_index in range(y_len):
            data_dict = data[y_index][x_index]
            for z_value in data_dict.keys():
                if z_value > z_max:
                    z_max = z_value
                if z_value < z_min:
                    z_min = z_value

    return (z_max, z_min)


def save_img(file_path: str, data: list[list[dict[int, int]]]) -> None:
    grid_size = 60
    line_width = 2
    fontsize = 25
    font = ImageFont.truetype("arial.ttf", fontsize)
    x_len = len(data[0])
    y_len = len(data)
    size = (
        x_len * (grid_size + line_width) + line_width,
        y_len * (grid_size + line_width) + line_width,
    )
    image = Image.new("RGB", size, grid_color)
    draw_img = ImageDraw.Draw(image)

    z_max, z_min = get_max_min(data)

    for x_index in range(x_len):
        for y_index in range(y_len):
            color = void_color
            data_dict = data[y_index][x_index]
            top_z_index = -999
            if data_dict != {}:
                top_z_index = max(data_dict.keys())
                if data_dict[top_z_index] == 0:
                    color = color_lerp(ground_colors, z_max, z_min, top_z_index)
                elif data_dict[top_z_index] == 1:
                    color = color_lerp(obt_colors, z_max, z_min, top_z_index)
                    # color = void_color
            # for z_value in data_dict.keys():
            #     color = (20, 20, 20)
            #     if data_dict[z_value] == 0:
            #         color = (200, 200, 200
            draw_img.rectangle(
                (
                    x_index * (grid_size + line_width) + line_width,
                    y_index * (grid_size + line_width) + line_width,
                    (x_index + 1) * (grid_size + line_width) - line_width,
                    (y_index + 1) * (grid_size + line_width) - line_width,
                ),
                fill=color,
            )
            draw_img.text(
                (
                    x_index * (grid_size + line_width) + grid_size // 2 - 5,
                    y_index * (grid_size + line_width) + grid_size // 2 - 5,
                ),
                str(top_z_index),
                fill=(0, 0, 0),
                font=font,
            )

    image.save(file_path)


data = read_map_csv("edit_me/Map4.csv")
save_img("out/test_image.png", data)
