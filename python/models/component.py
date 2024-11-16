class Component:
    def __init__(self, name, failure_probs, repair_cost, replace_entire_product=False):
        self.name = name
        self.failure_probs = failure_probs
        self.repair_cost = repair_cost
        self.replace_entire_product = replace_entire_product
        self.age = 0

    def get_failure_probability(self):
        if self.age < len(self.failure_probs):
            return self.failure_probs[self.age]
        else:
            return self.failure_probs[-1]

    def increment_age(self):
        self.age += 1

    def reset_age(self):
        self.age = 0