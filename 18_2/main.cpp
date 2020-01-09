/*
Дано множество точек на плоскости (x, y). Постройте выпуклую оболочку этого множества и вычислите ее периметр.
Вариант 2. С помощью алгоритма Джарвиса.
Обратите внимание: три и более точки из множества могут лежать на одной прямой.
*/
#include <iostream>
#include <math.h>
#include <unordered_set>

using namespace std;

namespace std {
    template<>
    struct hash<pair<double, double>> {
        size_t operator()(const pair<double, double> &p) const noexcept {
            return hash<double>()(p.first) * 7 + hash<double>()(p.second);
        };
    };
}


const double eps = 1e-15;
pair<double, double> get_vector(pair<double, double> start_point, pair<double, double> end_point)
{
    return make_pair(end_point.first - start_point.first, end_point.second - start_point.second);
}



double vector_multiplication(pair<double, double> vec1, pair<double, double> vec2)
{
    return vec1.first * vec2.second - vec2.first * vec1.second;
}

double dist(pair<double, double> point1, pair<double, double> point2)
{
    return sqrt(pow((point1.first - point2.first), 2) + pow((point1.second - point2.second), 2));
}

double convex_hull_perimetr(unordered_set<pair<double, double>> &points)
{
    double perimetr = 0;
    if (points.size() < 2)
        return 0;
    auto min_point = *points.begin();
    for (auto p : points)
    {
        if (min_point.second > p.second)
        {
            min_point = p;
        }
        else if (min_point.second == p.second && p.first < min_point.first)
        {
            min_point = p;
        }
    }

    pair<double, double> start_point = min_point;
    pair<double, double> current_point = start_point; //крайняя текущая точка выпуклой оболочки
    points.erase(start_point);
    bool is_first_cycle = true;
    while (start_point != current_point || is_first_cycle)
    {
        auto next_point = *points.begin();
        auto next_vector = get_vector(current_point, next_point);
        for (auto point : points)
        {
            if (point == next_point)
                continue;
            auto vector = get_vector(current_point, point);
            double vec_multipli = vector_multiplication(next_vector, vector);
            if (vec_multipli < -1. * eps)
            {
                next_point = point;
                next_vector = get_vector(current_point, next_point);
            }

            else if (vec_multipli < eps)
            {
                if (dist(current_point, point) > dist(current_point, next_point))
                {
                    next_point = point;
                    next_vector = get_vector(current_point, next_point);
                }
            }

        }

        if (is_first_cycle)
            points.insert(start_point);
        is_first_cycle=false;
        perimetr += dist(current_point, next_point);
        points.erase(next_point);
        current_point = next_point;
    }

    return perimetr;
}

int main() {
    int N;
    double  x, y;
    cin >> N;
    unordered_set<pair<double, double>> points{};
    for (int i = 0; i < N; i++) {
        cin >> x >> y;
        points.insert(make_pair(x, y));
    }
    cout << fixed;
    cout.precision(12);
    cout << convex_hull_perimetr(points);
    return 0;
}
