#include <Eigen/Core>
#include <Eigen/Dense>

#include <vector>
#include <string>
#include <fstream>

template<class T, int dim>
class MassSpringSystem{
public:
    using TV = Eigen::Matrix<T,dim,1>;
    
    std::vector<Eigen::Matrix<int,2,1> > segments;
    std::vector<T> m;
    std::vector<TV> x;
    std::vector<TV> v;
    T youngs_modulus;
    T damping_coeff;
    std::vector<bool> node_is_fixed;
    std::vector<T> rest_length;

    MassSpringSystem()
    {}

    void evaluateSpringForces(std::vector<TV>& f)
    {
        f.clear();
        f.resize(x.size(), TV::Zero());

	///////////////////////////////////////////////
	//  ASSIGNMENT ////////////////////////////////
	//  Add you code to build f /////////////////// 
	///////////////////////////////////////////////
        for(int i = 0; i < segments.size(); i++)
        {
            int idxi = segments[i](0, 0);
            int idxj = segments[i](1, 0);
            TV X = x[idxi] - x[idxj];
            T l = X.norm();
            TV fij = -youngs_modulus * (l / rest_length[i] - 1) * X / l;
            if (!node_is_fixed[idxi]) f[idxi] += fij;
            if (!node_is_fixed[idxj]) f[idxj] += -fij;
        }
    }

    void evaluateDampingForces(std::vector<TV>& f)
    {
        f.clear();
        f.resize(x.size(), TV::Zero());

	///////////////////////////////////////////////
	//  ASSIGNMENT ////////////////////////////////
	//  Add you code to build f /////////////////// 
	///////////////////////////////////////////////
        for (int i = 0; i < segments.size(); i++)
        {
            int idxi = segments[i](0, 0);
            int idxj = segments[i](1, 0);
            TV X = x[idxi] - x[idxj];
            T l = X.norm();
            TV n = X / l;
            TV fij = -damping_coeff * (n * n.transpose()) * (v[idxi] - v[idxj]);
            if (!node_is_fixed[idxi]) f[idxi] += fij;
            if (!node_is_fixed[idxj]) f[idxj] += -fij;
        }
    }

    void dumpPoly(std::string filename)
    {
        std::ofstream fs;
        fs.open(filename);
        fs << "POINTS\n";
        int count = 0;
        for (auto X : x) {
            fs << ++count << ":";
            for (int i = 0; i < dim; i++)
                fs << " " << X(i);
            if (dim == 2)
                fs << " 0";
            fs << "\n";
        }
        fs << "POLYS\n";
        count = 0;
        for (const Eigen::Matrix<int, 2, 1>& seg : segments)
            fs << ++count << ": " << seg(0) + 1 << " " << seg(1) + 1 << "\n"; // poly segment mesh is 1-indexed
        fs << "END\n";
        fs.close();
    }
    
};
