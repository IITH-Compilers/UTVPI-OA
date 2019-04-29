#if !defined(UTVPI_OA_FM_H)
#define UTVPI_OA_FM_H

#include <cassert>
#include <iostream>
#include <map>
#include <numeric>
#include <sstream>
#include <string>
#include <vector>

namespace fm {

template <class T>
struct Rational {
  T numerator;
  T denominator;

  Rational(T n = 0, T d = 1) {
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
struct VarBounds {
  bool posMaxFound = false;
  bool negMaxFound = false;
  Rational<T> posMax;
  Rational<T> negMax;
};

template <class T>
struct System {
  std::vector<std::vector<Rational<T>>> lines;
  std::vector<std::string> varLabels;
  unsigned nVars = 0, nLines = 0;

  void read(std::istream &in) {
    in >> nLines >> nVars;
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
        // Changing + c >= 0 to  >= -c
        if (j == nVars) rat = -rat;
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
      print_vector(out, line);
    }
  }

  static void print_vector(std::ostream &out,
                           const std::vector<Rational<T>> &v) {
    for (auto i = 0; i < v.size(); i++) {
      out << " ";
      if (i + 1 == v.size())
        (-v[i]).print(out);
      else
        v[i].print(out);
    }
    out << std::endl;
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
        assert(line[var] == Rational<T>(0));
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

  static bool findOA_f(const System<T> &system, System<T> &result,
                       std::map<std::string, unsigned> &varMap) {
    if (system.nVars == 2) {
      return findBounds(system, result, varMap);
    }
    bool r = findOA_f(system.removeVar(system.nVars - 1), result, varMap);
    if (!r) return false;
    r = findOA_g(system.removeVar(system.nVars - 2), result, varMap);
    if (!r) return false;
    return findOA_h(system, result, varMap);
  }

  static bool findOA_g(const System<T> &system, System<T> &result,
                       std::map<std::string, unsigned> &varMap) {
    if (system.nVars == 2) {
      return findBounds(system, result, varMap);
    }
    bool r = findOA_g(system.removeVar(system.nVars - 2), result, varMap);
    if (!r) return false;
    return findOA_h(system, result, varMap);
  }

  static bool findOA_h(const System<T> &system, System<T> &result,
                       std::map<std::string, unsigned> &varMap) {
    if (system.nVars == 2) {
      return findBounds(system, result, varMap);
    }
    return findOA_h(system.removeVar(0), result, varMap);
  }

  static bool findBounds(const System<T> &system, System<T> &result,
                         std::map<std::string, unsigned> &varMap) {
    assert(system.nVars == 2);
    auto res = simplifySingleVar(system.removeVar(1));
    if (res.first) {
      VarBounds<T> b = res.second;
      if (b.posMaxFound) {
        std::vector<Rational<T>> line(result.nVars + 1, 0);
        line[varMap[system.varLabels[0]]] = Rational<T>(1);
        line[result.nVars] = b.posMax;
        result.lines.push_back(line);
      }
      if (b.negMaxFound) {
        std::vector<Rational<T>> line(result.nVars + 1, 0);
        line[varMap[system.varLabels[0]]] = Rational<T>(-1);
        line[result.nVars] = b.negMax;
        result.lines.push_back(line);
      }
    } else {
      return false;
    }
    res = simplifySingleVar(system.removeVar(0));
    if (res.first) {
      VarBounds<T> b = res.second;
      if (b.posMaxFound) {
        std::vector<Rational<T>> line(result.nVars + 1, 0);
        line[varMap[system.varLabels[1]]] = Rational<T>(1);
        line[result.nVars] = b.posMax;
        result.lines.push_back(line);
      }
      if (b.negMaxFound) {
        std::vector<Rational<T>> line(result.nVars + 1, 0);
        line[varMap[system.varLabels[1]]] = Rational<T>(-1);
        line[result.nVars] = b.negMax;
        result.lines.push_back(line);
      }
    } else {
      return false;
    }
    System<T> rotated = system;
    rotated.varLabels[0] = system.varLabels[0] + "+" + system.varLabels[1];
    rotated.varLabels[1] = system.varLabels[0] + "-" + system.varLabels[1];
    for (auto i = 0; i < system.nLines; i++) {
      rotated.lines[i][0] = system.lines[i][0] + system.lines[i][1];
      rotated.lines[i][1] = system.lines[i][0] - system.lines[i][1];
      rotated.lines[i][2] = system.lines[i][2] * Rational<T>(2, 1);
    }
    res = simplifySingleVar(rotated.removeVar(1));
    if (res.first) {
      VarBounds<T> b = res.second;
      if (b.posMaxFound) {
        std::vector<Rational<T>> line(result.nVars + 1, 0);
        line[varMap[system.varLabels[0]]] = Rational<T>(1);
        line[varMap[system.varLabels[1]]] = Rational<T>(1);
        line[result.nVars] = b.posMax;
        result.lines.push_back(line);
      }
      if (b.negMaxFound) {
        std::vector<Rational<T>> line(result.nVars + 1, 0);
        line[varMap[system.varLabels[0]]] = Rational<T>(-1);
        line[varMap[system.varLabels[1]]] = Rational<T>(-1);
        line[result.nVars] = b.negMax;
        result.lines.push_back(line);
      }
    } else {
      return false;
    }
    res = simplifySingleVar(rotated.removeVar(0));
    if (res.first) {
      VarBounds<T> b = res.second;
      if (b.posMaxFound) {
        std::vector<Rational<T>> line(result.nVars + 1, 0);
        line[varMap[system.varLabels[0]]] = Rational<T>(1);
        line[varMap[system.varLabels[1]]] = Rational<T>(-1);
        line[result.nVars] = b.posMax;
        result.lines.push_back(line);
      }
      if (b.negMaxFound) {
        std::vector<Rational<T>> line(result.nVars + 1, 0);
        line[varMap[system.varLabels[0]]] = Rational<T>(-1);
        line[varMap[system.varLabels[1]]] = Rational<T>(1);
        line[result.nVars] = b.negMax;
        result.lines.push_back(line);
      }
    } else {
      return false;
    }
    return true;
  }

  static std::pair<bool, VarBounds<T>> simplifySingleVar(
      const System<T> &system) {
    assert(system.nVars == 1);
    VarBounds<T> varBounds;
    for (auto &line : system.lines) {
      if (line[0] > 0) {
        auto val = line[1] / line[0];
        if (varBounds.posMaxFound && varBounds.posMax > val) {
          varBounds.posMax = val;
        } else if (!varBounds.posMaxFound) {
          varBounds.posMaxFound = true;
          varBounds.posMax = val;
        }
      } else if (line[0] < 0) {
        auto val = line[1] / (-line[0]);
        if (varBounds.negMaxFound && varBounds.negMax > val) {
          varBounds.negMax = val;
        } else if (!varBounds.negMaxFound) {
          varBounds.negMaxFound = true;
          varBounds.negMax = val;
        }
      } else if (line[1] > 0) {
        return std::make_pair(false, varBounds);
      }
    }
    return std::make_pair(true, varBounds);
  }

  void printOA(std::ostream &out, bool vanilla = false) {
    System<T> result;
    result.varLabels = varLabels;
    result.nVars = nVars;
    std::map<std::string, unsigned> varMap;
    for (auto i = 0; i < nVars; i++) {
      varMap[varLabels[i]] = i;
    }
    bool r;
    if (vanilla)
      r = vanillaFMOA(*this, result, varMap);
    else
      r = findOA_f(*this, result, varMap);
    if (!r) {
      out << "Infeasible!" << std::endl;
    } else {
      result.print(out);
    }
  }

  static bool vanillaFMOA(const System<T> &system, System<T> &result,
                          std::map<std::string, unsigned> varMap) {
    for (auto i = 0; i < system.nVars; i++) {
      for (auto j = i + 1; j < system.nVars; j++) {
        System<T> temp = system;
        for (int k = 0; k < system.nVars; k++) {
          if (k != i && k != j) {
            temp = temp.removeVar(k);
          }
        }
        bool r = findBounds(temp, result, varMap);
        if (!r) {
          return false;
        }
      }
    }
    return true;
  }
};

}  // namespace fm

#endif  // UTVPI_OA_FM_H
