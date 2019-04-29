#if !defined(UTVPI_OA_FM_H)
#define UTVPI_OA_FM_H

#include <cassert>
#include <iostream>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace fm {

template <class T>
struct Rational {
  T numerator;
  T denominator;

  Rational(T n, T d) {
    numerator = n;
    denominator = d;
  }

  Rational(const Rational<T> &r) {
    numerator = r.numerator;
    denominator = r.denominator;
  }

  static Rational<T> read(std::istream &in) {
    Rational<T> rat(0, 1);
    std::string input;
    in >> input;
    auto found = input.find("/");
    if (found != std::string::npos) {
      std::string num = input.substr(0, found);
      std::string den = input.substr(found + 1);
      std::stringstream ssnum(num);
      ssnum >> rat.numerator;
      std::stringstream ssden(den);
      ssden >> rat.denominator;
    } else {
      std::stringstream ss(input);
      ss >> rat.numerator;
      rat.denominator = 1;
    }
    return rat;
  }

  void print(std::ostream &out) const {
    out << numerator;
    if (denominator != 1) {
      out << "/" << denominator;
    }
  }

  Rational<T> operator-() const {
    Rational<T> rat(-numerator, denominator);
    return rat;
  }

  Rational<T> operator=(const Rational<T> &other) {
    numerator = other.numerator;
    denominator = other.denominator;
    return *this;
  }

  Rational<T> operator+(const Rational<T> &other) const {
    Rational<T> rat(0, 1);
    T lcm = std::lcm(denominator, other.denominator);
    rat.denominator = lcm;
    rat.numerator = numerator * (lcm / denominator) +
                    other.numerator * (lcm / other.denominator);
    T gcd = std::gcd(rat.numerator, rat.denominator);
    rat.numerator /= gcd;
    rat.denominator /= gcd;
    return rat;
  }

  Rational<T> operator-(const Rational<T> &other) const {
    return *this + (-other);
  }

  Rational<T> reciprocal() const {
    Rational<T> rat(denominator, numerator);
    return rat;
  }

  Rational<T> operator*(const Rational<T> &other) const {
    Rational<T> rat(numerator * other.numerator,
                    denominator * other.denominator);
    T gcd = std::gcd(rat.numerator, rat.denominator);
    rat.numerator /= gcd;
    rat.denominator /= gcd;
    return rat;
  }

  Rational<T> operator/(const Rational<T> &other) const {
    return *this * other.reciprocal();
  }

  bool operator==(const Rational<T> &other) const {
    return numerator * other.denominator == other.numerator * denominator;
  }

  bool operator==(const T &other) const { return *this == Rational(other, 1); }

  bool operator!=(const Rational<T> &other) const { return !(*this == other); }

  bool operator!=(const T &other) const { return !(*this == other); }

  bool operator<=(const Rational<T> &other) const {
    if (denominator > 0 && other.denominator > 0)
      return numerator * other.denominator <= other.numerator * denominator;
    else {
      assert(false);
    }
  }

  bool operator<=(const T &other) const { return *this <= Rational(other, 1); }

  bool operator<(const Rational<T> &other) const {
    if (denominator > 0 && other.denominator > 0)
      return numerator * other.denominator < other.numerator * denominator;
    else {
      assert(false);
    }
  }

  bool operator<(const T &other) const { return *this < Rational(other, 1); }

  bool operator>=(const Rational<T> &other) const {
    if (denominator > 0 && other.denominator > 0)
      return numerator * other.denominator >= other.numerator * denominator;
    else {
      assert(false);
    }
  }

  bool operator>=(const T &other) const { return *this >= Rational(other, 1); }

  bool operator>(const Rational<T> &other) const {
    if (denominator > 0 && other.denominator > 0)
      return numerator * other.denominator > other.numerator * denominator;
    else {
      assert(false);
    }
  }

  bool operator>(const T &other) const { return *this > Rational(other, 1); }
};

template <class T>
struct System {
  std::vector<std::vector<Rational<T>>> lines;
  std::vector<std::string> varLabels;
  unsigned nVars = 0, nLines = 0;

  void read(std::istream &in) {
    in >> nVars >> nLines;
    nVars = nVars - 2;
    int type;
    T temp;

    for (auto i = 0; i < nVars; i++) {
      varLabels.push_back("x[" + std::to_string(i) + "]");
    }

    for (auto i = 0; i < nLines; i++) {
      std::vector<Rational<T>> line;
      in >> type;
      for (auto j = 0; j < nVars + 1; j++) {
        Rational<T> rat = Rational<T>::read(in);
        line.push_back(rat);
      }
      lines.push_back(line);
      if (type == 0) {
        std::vector<Rational<T>> line2 = line;
        for (auto j = 0; j < nVars + 1; j++) {
          line2[j] = -line2[j];
        }
        lines.push_back(line2);
      }
    }

    nLines = lines.size();
  }

  void print(std::ostream &out) const {
    if (lines.size() == 0) return;
    for (auto &var : varLabels) {
      out << " " << var;
    }
    out << " c" << std::endl;
    for (auto &line : lines) {
      out << 1;
      print_vector(line);
    }
  }

  System<T> removeVar(unsigned var) const {
    System<T> res;
    res.varLabels = varLabels;
    res.varLabels.erase(res.varLabels.begin() + var);
    for (auto i = 0; i < nLines; i++) {
      if (lines[i][var] == 0) {
        auto line = lines[i];
        line.erase(line.begin() + var);
        res.lines.push_back(line);
        continue;
      }
      if (lines[i][var] < 0) continue;
      for (auto j = 0; j < nLines; j++) {
        if (lines[j][var] >= 0 || i == j) continue;
        Rational<T> c1 = lines[i][var];
        Rational<T> c2 = lines[j][var];
        auto line = vectorLinearSum(-c2, lines[i], c1, lines[j]);
        line.erase(line.begin() + var);
        bool all_zeros = true;
        for (auto &r : line) {
          if (r != Rational<T>(0, 1)) {
            all_zeros = false;
            break;
          }
        }
        if (!all_zeros) res.lines.push_back(line);
      }
    }
    res.nVars = nVars - 1;
    res.nLines = res.lines.size();
    return res;
  }

  static void print_vector(const std::vector<Rational<T>> &v) {
    for (const Rational<T> &i : v) {
      std::cout << " ";
      i.print(std::cout);
    }
    std::cout << std::endl;
  }

  static std::vector<Rational<T>> vectorLinearSum(Rational<T> a,
                                                  std::vector<Rational<T>> x,
                                                  Rational<T> b,
                                                  std::vector<Rational<T>> y) {
    assert(x.size() == y.size());
    auto res = x;
    for (auto i = 0; i < res.size(); i++) {
      res[i] = a * x[i] + b * y[i];
    }
    return res;
  }

  static void printOA_f(const System<T> &system) {
    if (system.nVars == 2) {
      print_bounds(system);
      return;
    }
    printOA_f(system.removeVar(system.nVars - 1));
    printOA_g(system.removeVar(system.nVars - 2));
    printOA_h(system);
  }

  static void printOA_g(const System<T> &system) {
    if (system.nVars == 2) {
      print_bounds(system);
      return;
    }
    printOA_g(system.removeVar(system.nVars - 1));
    printOA_h(system);
  }

  static void printOA_h(const System<T> &system) {
    if (system.nVars == 2) {
      print_bounds(system);
      return;
    }
    printOA_h(system.removeVar(0));
  }

  static void print_bounds(const System<T> &system) {
    assert(system.nVars == 2);
    auto s0 = system.removeVar(0);
    simplifySingleVar(s0).print(std::cout);
    auto s1 = system.removeVar(1);
    simplifySingleVar(s1).print(std::cout);
    System<T> rotated = system;
    rotated.varLabels[0] = system.varLabels[0] + "+" + system.varLabels[1];
    rotated.varLabels[1] = system.varLabels[0] + "-" + system.varLabels[1];
    for (auto i = 0; i < system.nLines; i++) {
      rotated.lines[i][0] = system.lines[i][0] + system.lines[i][1];
      rotated.lines[i][1] = system.lines[i][0] - system.lines[i][1];
      rotated.lines[i][2] = system.lines[i][2] * Rational<T>(2, 1);
    }
    simplifySingleVar(rotated.removeVar(1)).print(std::cout);
    simplifySingleVar(rotated.removeVar(0)).print(std::cout);
  }

  static System<T> simplifySingleVar(const System<T> &system) {
    assert(system.nVars == 1);
    bool pos_max_found = false;
    Rational<T> pos_max(0, 1);
    bool neg_max_found = false;
    Rational<T> neg_max(0, 1);
    for (auto &line : system.lines) {
      if (line[0] > 0) {
        auto val = line[1] / line[0];
        if (pos_max_found && pos_max > val) {
          pos_max = val;
        } else {
          pos_max_found = true;
          pos_max = val;
        }
      } else if (line[0] < 0) {
        auto val = line[1] / (-line[0]);
        if (neg_max_found && neg_max > val) {
          neg_max = val;
        } else {
          neg_max_found = true;
          neg_max = val;
        }
      } else if (line[1] > 0) {
        std::cout << "Infeasible" << std::endl;
        return System<T>();
      }
    }

    System<T> res;
    res.nVars = 1;
    res.varLabels = system.varLabels;
    if (pos_max_found) {
      std::vector<Rational<T>> line;
      line.push_back(Rational<T>(1, 1));
      line.push_back(pos_max);
      res.lines.push_back(line);
    }
    if (neg_max_found) {
      std::vector<Rational<T>> line;
      line.push_back(Rational<T>(-1, 1));
      line.push_back(neg_max);
      res.lines.push_back(line);
    }
    res.nLines = res.lines.size();
    return res;
  }

  void printOA() { printOA_f(*this); }
};

}  // namespace fm

#endif  // UTVPI_OA_FM_H
