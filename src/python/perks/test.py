class Person:
    # --- Properties ---
    health: int = 100
    is_alive: bool = True
    name: str = "John Doe"

    # --- Methods ---
    def __init__(self, name: str, health: int = 100):
        self.name = name
        self.health = health
        print("Person created with name:", self.name)
        
    def speak(self):
        print("Hello, my name is", self.name)

    def take_damage(self, amount: int):
        self.health -= amount
        if self.health <= 0:
            self.is_alive = False
            print(self.name, "has fallen.")

class Peter(Person):
    occupation: str = "Programmer"

    def __init__(self):
        # Python's super() call is tricky to map 1-to-1.
        # The constructor logic in the generator now handles initialization.
        # We can simplify Python __init__ for now.
        self.name = "Peter"
        
    def speak(self):
        # Method override
        print("I'm Peter, and I'm a", self.occupation)