#include <execution>

int main()
{
  ParticleSystem particles(10);

  // set some values to start with
  std::transform(
      particles.begin(), particles.end(), particles.begin(),
      [p] {
        setRandom(p);    // set random values for location and use these values
                         // for color, velocity, and mass
        setCircular(p);  // set velocity perpendicular to the location vector
      }) ForceMatrix
  fm_gravity(ParticleSystem particles,
             std::function < Force(Particle, Particle)
                                 ff);  // creates a force matrix so that forces
                                       // between the two same particles do not
                                       // need to be computed twice (lazy
                                       // evaluation and Matrix is only half
                                       // full)

  // calculate forces (per frame)
  //
  fm_gravity.reset();

  std::transform(particles.begin(), particles.end(), particles.begin(),
                 [p, particles] {

    p << accumulate(p, fm_gravity) //gravitational forces between particles

      << accumulate(for_all(calc_force(particles, [p, other]{ //some other force defined by lamda (in this case, spring force)
      float constant = 1;
      float length = 10;
      glm::vec3 direction = other.location - p.location;
      float magnitude = -1 * constant * (length - glm::length(direction));

      return direction * magnitude;
          }))) //TODO not sure if "accumulate(for_all(calc_force(..." is too convoluted

      << glm::vec3{1,0,0} //"external" force e.g. wind

      << calc_force(p, Object(0,0,0), [p, o]{ //force that pulls p towards point 0,0,0
      return glm::normalize(o.location - p.location) * 0.1f;  //* strength
          };

                 });

  return 0;
}
