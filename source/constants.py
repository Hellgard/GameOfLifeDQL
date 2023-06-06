import pygame
from collections import namedtuple

pygame.init()
font = pygame.font.Font('data/arial.ttf', 25)
Point = namedtuple('Point', 'x, y')

White = (255, 255, 255)
Red = (255, 0, 0)
Black = (0, 0, 0) 
Blue = (0, 0, 255) 
Green = (0, 255, 0) 
Yellow = (255, 255, 0) 
Purple = (255, 0, 255) 
Cyan = (0, 255, 255) 
Brown = (165, 42, 42)
Grey = (128, 128, 128)

BLOCK_SIZE = 10
SPEED = 1
Forward = 100