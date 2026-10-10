# Perlin Noise GFX

A graphical visualizer for 2D Perlin noise, rendered as a smooth, color-mapped heatmap.

Animated:

<img width="500" height="500" alt="Perlin_Noise" src="https://github.com/user-attachments/assets/e81db79e-32c0-4e63-b4a8-df2f57179165" />

Image (Full Resolution):

<img width="500" height="500" alt="Perlin_1_Noise" src="https://github.com/user-attachments/assets/420687d2-bff0-43f0-a027-483ced6db10d" />

## About

Perlin noise is a gradient noise function developed by Ken Perlin in 1983. Unlike pure random noise, it produces smooth, continuous variation, which makes it useful for procedural textures, terrain generation, clouds, and other natural-looking patterns.

This project generates a 2D Perlin noise field and maps each value to a color (blue for low values, through cyan, green, and yellow, to red for high values) so the structure of the noise is easy to see.

## How It Works

1. **Grid of gradients:** The plane is divided into a lattice, and each lattice point is assigned a pseudo-random unit gradient vector.
2. **Dot products:** For any sample point, the offset vectors to the four surrounding lattice corners are dotted with those corners' gradients.
3. **Smooth interpolation:** The four results are blended using the fade curve `6t^5 - 15t^4 + 10t^3`, which gives zero first and second derivatives at the lattice points and removes visible grid artifacts.
4. **Color mapping:** The final noise value, normalized to `[0, 1]`, is mapped to a rainbow gradient.

## Getting Started

### Prerequisites

- Visual Studio 2022 or newer (the project uses a `.slnx` solution file)
- <!-- TODO: list any libraries, e.g. SFML, and the language/toolchain version -->

### Build and Run

```bash
git clone https://github.com/ryangriffith121/Perlin_Noise_GFX.git
cd Perlin_Noise_GFX
```

1. Open `PerlinNoise.slnx` in Visual Studio.
2. Select a configuration (Debug or Release) and platform (x64).
3. Build and run (`Ctrl+F5`).

## Project Structure

```
Perlin_Noise_GFX/
├── PerlinNoise/        # Source code
├── PerlinNoise.slnx    # Visual Studio solution
├── .gitattributes
├── .gitignore
└── README.md
```

## References

- Ken Perlin, ["An Image Synthesizer"](https://dl.acm.org/doi/10.1145/325165.325247), SIGGRAPH 1985
- Ken Perlin, ["Improving Noise"](https://dl.acm.org/doi/10.1145/566654.566636), SIGGRAPH 2002
