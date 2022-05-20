#include <array>
#include <random>
#include <iostream>
#include <cmath>
#include <tuple>
#include <functional>
#include <utility>
#include "progress.hpp"
#include <cstdint>

const size_t N = 500;
const double J = 1.0;

std::random_device rnd;
std::mt19937 gen(rnd());
std::uniform_real_distribution<double> unit_dist(0.0, 1.0);
auto rnd_unit = std::bind(unit_dist, std::ref(gen));


std::uniform_int_distribution<int> index_dist(0, N - 1);
auto rnd_index = std::bind(index_dist, std::ref(gen));

using chain_t = std::vector<signed char>;


static inline double energy(const chain_t& chain, int pos, double H)
{
    auto s = chain[pos];

    return -s * H - J * (chain[(pos - 1) % N] * s + chain[(pos + 1) % N] * s);
}

std::pair<double, double> set_chain(chain_t& chain, double T, double H)
{
    double E_mean = 0;
    double M_mean = 0;
    double b = 1.0 / T;
    uint64_t nrepeats = 1000;
    for (uint64_t i = 0; i <  nrepeats * N; ++i) {
        int pos = rnd_index();

        double E = energy(chain, pos, H);
        E_mean += E;
        M_mean += chain[pos];

        double factor = std::exp(-2 * E * b);
        double p = factor / (factor + 1);

        chain[pos] *= (1 - 2 * (rnd_unit() > p));
    }

    E_mean /= (nrepeats * N);
    M_mean /= (nrepeats * N);

    return std::pair(E_mean, M_mean);
}

double E(const chain_t& chain, double H)
{
    double ans = 0;

    for (int i = 0; i < N; ++i)
        ans += energy(chain, i, H); //rewrite this

    return ans / N;
}

double M(const chain_t& chain)
{
    double ans = 0;

    for (auto i : chain)
        ans += i;

    return ans / N;
}


std::tuple<std::vector<double>, std::vector<double>, std::vector<double>>
get_E_M(double H)
{
    double Tmin = 0.05;
    double Tmax = 10;
    int nsteps = 100;
    double dT = (Tmax - Tmin) / nsteps;

#ifdef DEBUG
    progressbar bar(nsteps);
#endif

    std::vector<double> Ts(nsteps), Es(nsteps), Ms(nsteps);

    std::cout << "H = " << H << std::endl;

#ifdef MULTITHREAD
#pragma omp parallel for
#endif
    for (int i = 0; i < nsteps; ++i) {
        chain_t chain(N, 1);
#ifdef DEBUG
        bar.update();
#endif
        double T = Tmin + i * dT;

        auto p = set_chain(chain, T, H);
        Ts[i] = T;
        Es[i] = p.first;
        Ms[i] = p.second;
    }

    return std::tuple{Ts, Es, Ms};
}

std::ostream&
operator<<(std::ostream& o, std::vector<double>& v)
{
    for (double d : v)
        o << d << ' ';
    return o;
}

std::ostream&
operator<<(
        std::ostream& o,
        const std::tuple<std::vector<double>,
        std::vector<double>,
        std::vector<double>>& t
        )
{
    auto Ts = std::get<0>(t);
    auto Es = std::get<1>(t);
    auto Ms = std::get<2>(t);

    if (Ts.size() != Es.size() || Es.size() != Ms.size())
        return o;

    o << "Ts Es Ms" << std::endl;

    size_t size = Ts.size();

    for (size_t i = 0; i < size; ++i)
        o << Ts[i] << " " << Es[i] << " " << Ms[i] << std::endl;

    return o;
}


int main(int argc, char* argv[])
{
    if (argc < 2)
        return 1;

    double H = atof(argv[1]);

    auto t = get_E_M(H);
    std::cout << t << std::endl;

    return 0;
}
