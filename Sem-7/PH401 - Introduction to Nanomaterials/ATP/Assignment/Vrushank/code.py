# writing a computer program to deduce the total number of atoms and surface atoms for different shell of cuboctahedral / spherical  shape .Plot % of atoms in bulk/ surface versus particle size.

import matplotlib.pyplot as plt


def cubo_octahedral_total(k):
    ans = (10*(k**3) + 15*(k**2) + 11*k + 3)/3
    return ans


def cubo_octahedral_surface(k):
    ans = 10*(k**2) + 2
    return ans


def spherical_total(k):
    ans = (10*(k**3) - 15*(k**2) + 11*k - 3)/3
    return ans


def spherical_surface(k):
    ans = 10*(k**2) + 12 - 20*k
    return ans


def main():

    option = input("Enter 1 for CuboOctahedral , 2 for Spherical: ")
    
    try:
        option = int(option)
    except:
        print("enter correct input!!")
        return

    if not (option == 1 or option == 2):
        print("Enter correct input !!")
        return
    
    atoms_surface = []
    atoms_bulk = []
    sizes_of_particle = [i + 1 for i in range(50)]

#calculating surface and bulk atoms
    for particle_size in sizes_of_particle:
        if option == 1:
            x = (cubo_octahedral_surface(particle_size) /
                    cubo_octahedral_total(particle_size))*100
        else:
            x = (spherical_surface(particle_size) /
                    spherical_total(particle_size))*100

        atoms_surface.append(x)
        atoms_bulk.append(100 - x)

#plotting the graphs 
    plt.scatter(sizes_of_particle, atoms_bulk, label="bulk atoms")
    plt.scatter(sizes_of_particle, atoms_surface, label="surface atoms")
    plt.legend(loc='center right')
    plt.xlabel("particle size")
    plt.ylabel("% of atoms in bulk/surface")
    if option == 1:
        plt.title("CuboOctahedral")
    else:
        plt.title("Spherical")
    plt.show()


if __name__ == "__main__":
    main()
