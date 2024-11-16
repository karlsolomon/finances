import numpy as np, pandas as pd, seaborn as sns
import matplotlib.pyplot as plt

from data.components_data import framework_components, macbook_components
from models.product import Product
from simulations.simulate import simulate_repair_costs

def main(YEARS:int):
    # Create Product instances
    framework_laptop = Product(
        name='Framework Laptop',
        initial_cost=1500,
        components=framework_components
    )

    macbook_pro = Product(
        name='MacBook Pro',
        initial_cost=2500,
        components=macbook_components
    )

    # Simulation parameters
    NUM_SIMULATIONS = 100000
    YEARS = YEARS

    # Run simulations
    framework_costs = simulate_repair_costs(framework_laptop, NUM_SIMULATIONS, YEARS)
    macbook_costs = simulate_repair_costs(macbook_pro, NUM_SIMULATIONS, YEARS)

    # Analyze results
    frame,mb = analyze_results(framework_costs, macbook_costs, YEARS)
    return frame,mb

def analyze_results(framework_costs, macbook_costs, years):
    # Calculate averages
    average_framework_cost = np.mean(framework_costs)
    average_macbook_cost = np.mean(macbook_costs)

    print(f"\nAverage Total Cost over {years} years for Framework Laptop: ${average_framework_cost:.2f}")
    print(f"Average Total Cost over {years} years for MacBook Pro: ${average_macbook_cost:.2f}")
    
    return [{'Laptop':'Framework','Year': years,'Cost':average_framework_cost,'Avg_Yearly Cost':average_framework_cost / years},
            {'Laptop':'MacBookPro','Year': years,'Cost':average_macbook_cost,'Avg_Yearly Cost':average_macbook_cost / years},
    ]

    # Plot results
    #plot_cost_distribution(framework_costs, macbook_costs, years)

# def plot_cost_distribution(framework_costs, macbook_costs, years):
#     plt.figure(figsize=(12, 6))
#     plt.hist(framework_costs, bins=20, alpha=0.5, label='Framework Laptop')
#     plt.hist(macbook_costs, bins=20, alpha=0.5, label='MacBook Pro')
#     plt.xlabel(f'Total Cost over {years} Years ($)')
#     plt.ylabel('Frequency')
#     plt.title('Distribution of Total Costs over 10 Years')
#     plt.legend()
#     plt.show()

def plot_results_seaborn(df):
    plt.figure(figsize=(10, 6))
    sns.lineplot(data=df, x='Year', y='Cost', hue='Laptop', marker='o')
    plt.xlabel('Year')
    plt.ylabel('Average Total Cost ($)')
    plt.title('Average Total Cost Over Years')
    plt.grid(True)
    plt.show()

if __name__ == '__main__':
    all_results = []
    
    for years in range(1, 11):  # Simulate for years 1 to 10
        data = main(years)
        all_results.extend(data)  # Add the results to the list
    
    # Create DataFrame from the list of results
    df = pd.DataFrame(all_results)
    print(df)
    
    # Plot the results using Seaborn
    plot_results_seaborn(df)