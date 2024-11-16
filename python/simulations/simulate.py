import numpy as np
from models.component import Component
from models.product import Product
from utils.helpers import get_failure_probability

def simulate_repair_costs(product, num_simulations=100, years=10):
    total_costs = []
    for sim in range(num_simulations):
        total_cost = product.initial_cost

        if product.name == 'MacBook Pro':
            laptop_age = 0
        else:
            # Reset ages for all components
            for component in product.components:
                component.reset_age()

        for year in range(years):
            if product.name == 'MacBook Pro':
                total_cost, laptop_age = simulate_macbook_year(
                    product, total_cost, laptop_age
                )
            else:
                total_cost = simulate_framework_year(
                    product, total_cost
                )
        total_costs.append(total_cost)
    return total_costs

def simulate_macbook_year(product, total_cost, laptop_age):
    laptop_year = laptop_age
    laptop_age += 1

    for component in product.components:
        failure_prob = get_failure_probability(component, laptop_year)
        if np.random.rand() < failure_prob:
            if component.replace_entire_product:
                # Replace entire laptop
                total_cost += product.initial_cost
                laptop_age = 0  # Reset laptop age
                break  # Exit loop since laptop is replaced
            else:
                # Replace component (Screen)
                total_cost += component.repair_cost # TODO: reset component age to 0
    return total_cost, laptop_age

def simulate_framework_year(product, total_cost):
    for component in product.components:
        failure_prob = component.get_failure_probability()
        if np.random.rand() < failure_prob:
            # Replace component
            total_cost += component.repair_cost
            component.reset_age()
        else:
            component.increment_age()
    return total_cost