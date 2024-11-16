def get_failure_probability(component, age):
    if age < len(component.failure_probs):
        return component.failure_probs[age]
    else:
        return component.failure_probs[-1]