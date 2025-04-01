import pygame
from math import cos, sin, sqrt, pi
pygame.init()

pi2 = 2 * pi
pi05 = pi / 2

N = 6
radius = 40
start_x = 50
start_y = 50


BLACK = (0, 0, 0)
BLUE = (50, 125, 160)
GREEN = (0, 255, 0)
RED = (255, 0, 0)
BROWN = (81, 62, 62)
YELLOW = (252, 242, 0)

def pixel_to_hex(x, y, hex_size):
    q = (sqrt(3) / 3 * x - 1/3 * y) / hex_size
    r = (2/3 * y) / hex_size
    return hex_round(q, r)

def hex_round(q, r):
    s = -q - r
    
    q_round = round(q)
    r_round = round(r)
    s_round = round(s)
    
    q_diff = abs(q_round - q)
    r_diff = abs(r_round - r)
    s_diff = abs(s_round - s)
    
    if q_diff > r_diff and q_diff > s_diff:
        q_round = -r_round - s_round
    elif r_diff > s_diff:
        r_round = -q_round - s_round
    else:
        s_round = -q_round - r_round
    
    return (q_round, r_round)

def hex_to_pixel(q, r, hex_size):
    x = hex_size * sqrt(3) * (q + r/2)
    y = hex_size * (3/2) * r
    return (x, y)

def offset_to_axial(col, row):
    q = col - (row - (row & 1)) // 2
    r = row
    return q, r

def draw_ngon(Surface, color, n, radius, position, width = 0):
    return pygame.draw.polygon(
        Surface,
        color,
        [(cos(pi05 + i / n * pi2) * radius + position[0], sin(pi05 + i / n * pi2) * radius + position[1]) for i in range(0, n)],
        width
    )

def draw_grid(sel):
    # Draw grid
    for row in range(N):
        for col in range(N):
            q, r = offset_to_axial(col, row)
            x_pixel, y_pixel = hex_to_pixel(q, r, radius)
            x_pixel += start_x
            y_pixel += start_y
            
            draw_ngon(win, BROWN, 6, radius, (x_pixel, y_pixel))
    
    # For hovering
    if (sel):
        q, r = sel
        x_pixel, y_pixel = hex_to_pixel(q, r, radius)
        x_pixel += start_x
        y_pixel += start_y

        draw_ngon(win, YELLOW, 6, radius, (x_pixel, y_pixel), 3)


win = pygame.display.set_mode((500, 500))

sel = None
loop = True
while loop:
    for event in pygame.event.get():
        if (event.type == pygame.QUIT):
            loop = False
        if (event.type == pygame.MOUSEMOTION):
            mouse_x, mouse_y = event.pos
            rel_x = mouse_x - start_x
            rel_y = mouse_y - start_y
            sel = pixel_to_hex(rel_x, rel_y, radius)

    
    win.fill(BLUE)
    draw_grid(sel)

    pygame.display.flip()

pygame.quit()
