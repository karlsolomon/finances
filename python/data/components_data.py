from models.component import Component

macbook_components = [
    Component(
        name='CPU',
        failure_probs=[.05, .10, .15, .2, .4, .6, .8, .99, 1.0, 1.0],
        repair_cost=2500,
        replace_entire_product=True  # Replaces entire laptop
    ),
    Component(
        name='RAM',
        failure_probs=[.05, .10, .2, .4, .6, .8, 0.9, 1.0, 1.0, 1.0],
        repair_cost=2500,
        replace_entire_product=True
    ),
    Component(
        name='GFX Card',
        failure_probs=[.05, .1, .15, .2, .3, .4, .6, .8, 0.9, 1.0],
        repair_cost=2500,
        replace_entire_product=True
    ),
    Component(
        name='Screen',
        failure_probs=[.10, .15, .2, .4, .6, .8, 0.9, 1.0, 1.0, 1.0],
        repair_cost=100,
        replace_entire_product=False  # Screen can be replaced individually
    ),
    Component(
        name='Motherboard',
        failure_probs=[.05, .1, .15, .2, .6, .7, .8, 0.9, 1.0, 1.0],
        repair_cost=2500,
        replace_entire_product=True
    ),
    Component(
        name='Memory',
        failure_probs=[.05, .08, .15, .25, .45, .65, .85, 0.95, 1.0, 1.0],
        repair_cost=2500,
        replace_entire_product=True
    ),
    Component(
        name='Power Supply',
        failure_probs=[.04, .08, .12, .2, .3, .6, .8, 0.9, 1.0, 1.0],
        repair_cost=2500,
        replace_entire_product=True
    )
]

# Define Framework Laptop components
framework_components = [
    Component(
        name='CPU',
        failure_probs=[.05, .10, .15, .2, .25, .4, .6, .8, 0.9, 1.0],
        repair_cost=300
    ),
    Component(
        name='RAM',
        failure_probs=[.05, .10, .2, .4, .6, .8, 0.9, 1.0, 1.0, 1.0],
        repair_cost=100
    ),
    Component(
        name='GFX Card',
        failure_probs=[.05, .1, .15, .2, .3, .4, .6, .8, 0.9, 1.0],
        repair_cost=1000
    ),
    Component(
        name='Screen',
        failure_probs=[.10, .15, .2, .4, .6, .8, 0.9, 1.0, 1.0, 1.0],
        repair_cost=200
    ),
    Component(
        name='Motherboard',
        failure_probs=[.05, .1, .15, .2, .4, .6, .8, 0.9, 1.0, 1.0],
        repair_cost=200
    ),
    Component(
        name='Memory',
        failure_probs=[.05, .08, .15, .25, .45, .65, .85, 0.95, 1.0, 1.0],
        repair_cost=200
    ),
    Component(
        name='Power Supply',
        failure_probs=[.04, .08, .12, .2, .3, .6, .8, 0.9, 1.0, 1.0],
        repair_cost=200
    )
]