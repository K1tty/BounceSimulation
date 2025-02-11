# Overview
Playground for extensive optimization of the next task:
- A 2D world containing **100k Walls** and **10k Balls**
- Walls represented as 2D line segments
- Balls represented as material points
- Balls bounce off Walls
- A Wall must be destroyed when a Ball bounces off it
- No Balls allowed to pass through walls

 # Performance Target
 - **60 fps** on **MacBook Pro 2016**
   - CPU: Intel(R) Core(TM) i7-6700HQ CPU @ 2.60GHz (4-core, Turbo-Boost 3.1 Ghz)
   - GPU: AMD Radeon Pro 450

# Solutions to Try
- [x] Brut-force: O(n<sup>2</sup>) + cache-friendly + branchless code
- [ ] QuadTree/BVH: speed up the nearest search problem using the optimal data structures to store walls/balls
- [ ] Time-slicing: throttle calculations to achieve desired frame rate
- [ ] Bentley-Ottmann algorithm: O((n+k)*log(n))
- [ ] OpenCL/ComputeShaders
