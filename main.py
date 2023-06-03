from source.classGame import GameOfLife
from source.constants import Point
import pygame


def __main__():
    game = GameOfLife(Point(40, 40))
    print("Game of life")
    game.map.draw()
    game.game()

__main__()