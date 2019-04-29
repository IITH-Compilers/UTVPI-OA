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

  Rational<T> operator-() {
    Rational<T> rat(-numerator, denominator);
    return rat;
  }

  Rational<T> operator=(const Rational<T> &other) {
    numerator = other.numerator;
    denominator = other.denominator;
    return *this;
  }

  Rational<T> operator+(const Rational<T> &other) {
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

  Rational<T> operator-(const Rational<T> &other) { return *this + (-other); }

  Rational<T> reciprocal() {
    Rational<T> rat(denominator, numerator);
    return rat;
  }

  Rational<T> operator*(const Rational<T> &other) {
    Rational<T> rat(numerator * other.numerator,
                    denominator * other.denominator);
    T gcd = std::gcd(rat.numerator, rat.denominator);
    rat.numerator /= gcd;
    rat.denominator /= gcd;
    return rat;
  }

  Rational<T> operator/(const Rational<T> &other) {
    return *this * other.reciprocal();
  }

  bool operator==(const Rational<T> &other) {
    return numerator * other.denominator == other.numerator * denominator;
  }

  bool operator==(const T &other) { return *this == Rational(other, 1); }

  bool operator<=(const Rational<T> &other) {
    if (denominator > 0 && other.denominator > 0)
      return numerator * other.denominator <= other.numerator * denominator;
    else {
      assert(false);
    }
  }

  bool operator<=(const T &other) { return *this <= Rational(other, 1); }

  bool operator<(const Rational<T> &other) {
    if (denominator > 0 && other.denominator > 0)
      return numerator * other.denominator < other.numerator * denominator;
    else {
      assert(false);
    }
  }

  bool operator<(const T &other) { return *this < Rational(other, 1); }

  bool operator>=(const Rational<T> &other) {
    if (denominator > 0 && other.denominator > 0)
      return numerator * other.denominator >= other.numerator * denominator;
    else {
      assert(false);
    }
  }

  bool operator>=(const T &other) { return *this >= Rational(other, 1); }

  bool operator>(const Rational<T> &other) {
    if (denominator > 0 && other.denominator > 0)
      return numerator * other.denominator > other.numerator * denominator;
    else {
      assert(false);
    }
  }

  bool operator>(const T &other) { return *this > Rational(other, 1); }
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

  void print(std::ostream &out) {
    for (auto &var : varLabels) {
      out << " " << var;
    }
    out << " c" << std::endl;
    for (auto &line : lines) {
      out << 1;
      print_vector(line);
    }
  }

  System<T> removeVar(unsigned var) {
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
        res.lines.push_back(line);
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

  System<T> findOA() {}
};

}  // namespace fm

#endif  // UTVPI_OA_FM_H
