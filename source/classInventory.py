from source.classEnum import Direction, Ressource

class Inventory:
    def __init__(self):
        self.food = 0
        self.linemate = 0
        self.deraumere = 0
        self.sibur = 0
        self.mendiane = 0
        self.phiras = 0
        self.thystame = 0
        # EvolutionRequirements[level][ressource][numberrequiered] 

    def add(self, ressource, number):
        if ressource == Ressource.FOOD:
            self.food += number
        elif ressource == Ressource.LIMEMATE:
            self.linemate += number
        elif ressource == Ressource.DERAUMERE:
            self.deraumere += number
        elif ressource == Ressource.SIBUR:
            self.sibur += number
        elif ressource == Ressource.MENDIANE:
            self.mendiane += number
        elif ressource == Ressource.PHIRAS:
            self.phiras += number
        elif ressource == Ressource.THYSTAME:
            self.thystame += number
    
    def remove(self, ressource, number):
        if ressource == Ressource.FOOD:
            self.food -= number
        elif ressource == Ressource.LIMEMATE:
            self.linemate -= number
        elif ressource == Ressource.DERAUMERE:
            self.deraumere -= number
        elif ressource == Ressource.SIBUR:
            self.sibur -= number
        elif ressource == Ressource.MENDIANE:
            self.mendiane -= number
        elif ressource == Ressource.PHIRAS:
            self.phiras -= number
        elif ressource == Ressource.THYSTAME:
            self.thystame -= number

    def removes(self, ressources, numbers):
        # ressources = [Ressource.FOOD, Ressource.LIMEMATE, Ressource.DERAUMERE, Ressource.SIBUR, Ressource.MENDIANE, Ressource.PHIRAS, Ressource.THYSTAME]
        # numbers = [1, 1, 1, 1, 1, 1, 1]
        for i in range(len(ressources)):
            self.remove(ressources[i], numbers[i])

    def get(self, ressource):
        if ressource == Ressource.FOOD:
            return self.food
        elif ressource == Ressource.LIMEMATE:
            return self.linemate
        elif ressource == Ressource.DERAUMERE:
            return self.deraumere
        elif ressource == Ressource.SIBUR:
            return self.sibur
        elif ressource == Ressource.MENDIANE:
            return self.mendiane
        elif ressource == Ressource.PHIRAS:
            return self.phiras
        elif ressource == Ressource.THYSTAME:
            return self.thystame
        
    def get_all(self):
        return [self.food, self.linemate, self.deraumere, self.sibur, self.mendiane, self.phiras, self.thystame]
    
    def get_rocks(self):
        return [self.linemate, self.deraumere, self.sibur, self.mendiane, self.phiras, self.thystame]
    
    def show(self):
        print("Food: " + str(self.food) + "\nLinemate: " + str(self.linemate) + "\nDeraumere: " + str(self.deraumere) + "\nSibur: " + str(self.sibur) + "\nMendiane: " + str(self.mendiane) + "\nPhiras: " + str(self.phiras) + "\nThystame: " + str(self.thystame))
    