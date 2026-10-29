# Nibbler

A Snake game written in **C++17**, where the graphics are not part of the game itself: each GUI lives in its own **shared library** that the game loads at runtime with `dlopen`. You can switch between **raylib**, **SDL2** and **SFML** in the middle of a game.

## Features

- Classic Snake: the snake moves on its own, you only steer it
- Three interchangeable graphical libraries, switchable live with `1`, `2` and `3`
- Bonus features:
  - **Two-player mode** on the same board (single player stays available)
  - **Trap** only available with "--multi"
  - **Score limit**: the first snake to reach it wins
  - **Three kinds of apples** worth 10, 30 or 50 points, with different spawn rates
  - **Food Disappear** every 10 seconds
  - **Random obstacles**
  - **Sounds and music**, also handled by a dynamic library
  - Adjustable speed and instant restart

## Build

Requirements: Linux, a C++17 compiler and `make`.

```sh
$> make
```

## Usage

```sh
$> ./nibbler <width> <height> [options]
```

`width` and `height` are the size of the board in cells (from 15 to 50).

| Option | Description |
| --- | --- |
| `--multi` | Two snakes on the same board |
| `--enable-trap` | Can sacrifice part of their body to create an obstacle in its place |
| `--limit-score <n>` | The first snake to reach `n` points wins |
| `--food-variance` | Red (10 points), green (30) and yellow (50) apples; yellow ones are the rarest |
| `--enable-obstacles <n>` | Add `n` random obstacles (the snake dies if it hits one) |
| `--enable-sounds` | Background music, eating and death sounds |
| `--food-disappear` | Every 10 seconds |
| `--help` | Print the usage |

Examples:

```sh
./nibbler 30 20
./nibbler 30 20 --multi --food-variance
./nibbler 40 30 --enable-obstacles 10 --limit-score 200 --enable-sounds
```

## Controls

| Key | Action |
| --- | --- |
| `W` `A` `S` `D` | Steer snake 1 |
| Arrow keys | Steer snake 2 in `--multi` mode (snake 1 otherwise) |
| `1` `2` `3` | Switch to raylib / SDL2 / SFML |
| `=` / `-` | Speed up / slow down |
| `R` / `RCtrl` | Player 1 shrink / Player 2 shrink |
| `Enter` | Restart after the game is over |
| `Esc` | Quit |