from pulp import *
import sys


def solve_toy_distribution(input_data):
    # Parse input data
    lines = input_data.strip().split("\n")
    n_factories, n_countries, n_children = map(int, lines[0].split())

    # Current line index
    current_line = 1

    # Read factories data
    factories = {}  # (factory_id, country_id, max_stock)
    for i in range(n_factories):
        factory_id, country_id, max_stock = map(int, lines[current_line + i].split())
        factories[factory_id] = {"country": country_id, "stock": max_stock}

    current_line += n_factories

    # Read countries data
    countries = {}  # (country_id, max_export, min_gifts)
    for i in range(n_countries):
        country_id, max_export, min_gifts = map(int, lines[current_line + i].split())
        countries[country_id] = {"max_export": max_export, "min_gifts": min_gifts}

    current_line += n_countries

    # Read children requests
    children = {}  # (child_id, country_id, [requested_factories])
    for i in range(n_children):
        child_data = list(map(int, lines[current_line + i].split()))
        child_id, country_id = child_data[0:2]
        requested_factories = child_data[2:]
        children[child_id] = {"country": country_id, "requests": requested_factories}

    # START OF SOLUTION
    # Create the linear programming problem
    prob = LpProblem("Toy_Distribution", LpMaximize)

    # Decision variables
    # x[c][f] = 1 if child c receives a toy from factory f, 0 otherwise
    x = LpVariable.dicts(
        "gift",
        ((c, f) for c in children.keys() for f in children[c]["requests"]),
        cat="Binary",
    )

    # Objective: Maximize number of children receiving gifts
    prob += lpSum(x[c, f] for c in children.keys() for f in children[c]["requests"])

    # Constraints

    # 1. Each child receives at most one gift
    for c in children.keys():
        prob += lpSum(x[c, f] for f in children[c]["requests"]) <= 1

    # 2. Factory stock limits
    for f in factories.keys():
        prob += (
            lpSum(x[c, f] for c in children.keys() if f in children[c]["requests"])
            <= factories[f]["stock"]
        )

    # 3. Country export limits
    for p in countries.keys():
        relevant_factories = [
            f for f in factories.keys() if factories[f]["country"] == p
        ]
        prob += (
            lpSum(
                x[c, f]
                for c in children.keys()
                for f in (set(children[c]["requests"]) & set(relevant_factories))
                if children[c]["country"] != p
            )
            <= countries[p]["max_export"]
        )

    # 4. Country minimum gifts requirement
    for p in countries.keys():
        prob += (
            lpSum(
                x[c, f]
                for c in children.keys()
                for f in children[c]["requests"]
                if children[c]["country"] == p
            )
            >= countries[p]["min_gifts"]
        )

    # Solve the problem
    prob.solve()

    # Check if a solution was found
    if LpStatus[prob.status] == "Optimal":
        return int(value(prob.objective))
    else:
        return -1


def main():
    # Read input from stdin
    input_data = sys.stdin.read()

    # Solve the problem and print the result
    result = solve_toy_distribution(input_data)
    print(result)


if __name__ == "__main__":
    main()
