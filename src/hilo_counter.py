HILO = {
    "2": 1,
    "3": 1,
    "4": 1,
    "5": 1,
    "6": 1,
    "7": 0,
    "8": 0,
    "9": 0,
    "10": -1,
    "J": -1,
    "Q": -1,
    "K": -1,
    "A": -1,
}

class HiLoCounter():
    def __init__(self, decks=1):
        self.count = 0
        self.decks = decks

    def process(cards: [str]) -> float:
        self.count += sum(HILO[c[:-1]] for c in cards) / self.decks
        return self.count

