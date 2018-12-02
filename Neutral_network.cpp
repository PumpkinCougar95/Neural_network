// Weight Predictor.cpp : This file contains the 'main' function. Program execution begins and ends there.
// My First AI in c++, lets hope it works.

#include<iostream>
#include<stdlib.h>
#include<cmath>
#include<conio.h>
#include<fstream>
#include<iomanip>
#include<vector>

using namespace std;
#define DATA_SIZE 3 
#define Features 3
#define DATAFILE "maDATA.dat"
#define ERROR -1 

int SIZE = DATA_SIZE;

double approx(double a)
{
	if (abs(a) < 0.000001)
		a = 0;

	return a;
}

class matrix
{
private:
    // create a dynamically allocated array of pointers each of which pointing to another dynamically allocated array = 2D ARRAY 
	double trace;
	double det;
	int id;

	void createid()
	{
		fstream file;
		file.open(DATAFILE, ios::in | ios::binary);

		if (file.is_open())
		{
			file.seekg( - sizeof(matrix) , ios::end);
			file.read((char *)this, sizeof(matrix));
			id++;
		}
		else
		{
			id = 1;
		}
		file.close();
	}

protected:

	int rows;
	int columns;
	double* *m;
	
	double cofactor(int i, int j)
	{
		if (rows != columns)
		{
			return -1;
		}

		if (rows == 1)
		{
			return m[0][0];
		}

		double ** a = new double *[rows - 1];

		for (int i = 0; i < columns - 1; i++)
		{
			a[i] = new double[columns - 1];
		}

		int temp1, temp2;

		for (int l = 0; l < rows; l++)
			for (int n = 0; n < columns; n++)
			{
				temp1 = l;
				temp2 = n;
				if (l != i && n != j)
				{
					if (l > i)
					{
						temp1--;
					}
					if (n > j)
					{
						temp2--;
					}

					a[temp1][temp2] = m[l][n];
				}
			}

		matrix t(a, rows - 1 , columns -1);

		return pow(-1, (i + j)) * t.getdet();
	}

public:

	matrix(double **a, int row , int col)
	{
		rows = row;
		columns = col;
		m = new double*[rows];

		for (int i = 0; i < rows; i++)
		{
			m[i] = new double[columns];
		}

		m = a;
		id = 0;
		// createid(); // matrixes can have the same id if some are not saved. so better to not call this. id = 0 means no id
	}

	matrix(int row, int col)
	{
		rows = row;
		columns = col;
	
		m = new double*[rows];

		for (int i = 0; i < rows; i++)
		{
			m[i] = new double[columns];
		}

		id = 0;
	}

	int load(int i) // i is the id
	{
		fstream file;
		file.open(DATAFILE , ios::in|ios::binary);
		if (!file.is_open())
		{
			file.close();
			return ERROR;
		}
		file.seekg((i - 1) * sizeof(matrix) , ios::beg);
		file.read((char *)this, sizeof(matrix));
		file.close();
		return 
	}
	
	int getid()
	{
		return id;
	}

	int save()
	{
		createid();
		fstream infile;

		infile.open(DATAFILE, ios::app | ios::binary);

		clean();

		infile.write((char *)this, sizeof(matrix));
		infile.close();
	}

	void clean()
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
			{
				m[i][j] = approx(m[i][j]);
			}
	}

	int ftrace()
	{
		if (rows != columns)
		{
			trace = ERROR;
			return ERROR;
		}

		trace = 0;

		for (int i = 0; i < rows; i++)
		{
			trace += m[i][i];
		}
		return 0;
	}

	double gettrace()
	{
		return trace;
	}

	int getrows()
	{
		return rows;
	}

	int getcols()
	{
		return columns;
	}

	double getdet()
	{
		return det;
	}

	double element(int i, int j)
	{
		return m[i][j];
	}

	void find_det()
	{
		if (rows != columns)
		{
			det = ERROR;
			return;
		}

		det = 0;

		if (rows == 1)
		{
			det = m[0][0];
			return;
		}

		for (int i = 0; i < rows; i++)
		{
			det += m[0][i] * cofactor(0, i);
		}

		clean();
	}

	void cofactormatrix() 
	{
		if (rows != columns)
		{
			return;
		}

		double ** cm = new double*[rows];

		for (int i = 0; i < columns; i++)
		{
			cm[i] = new double[columns];
		}

		for (int i = 0; i < rows; i++)
		{
			for (int j = 0; j < columns; j++)
			{
				cm[i][j] = cofactor(i, j);
			}
		}

		m = cm;
	}

	matrix T()
	{
		double **r = new double* [columns];
		for (int i = 0; i < columns; i++)
		{
			r[i] = new double[rows];
		}
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
			{
				r[j][i] = m[i][j];
			}
		matrix temp(r , columns , rows);
		
		return temp;
	}

	int inverse()
	{
		find_det();
		if (det == 0 || rows != columns)
		{
			cout << " NOT INVERTIBLE! " << endl;
			return ERROR;
		}

		matrix* temp = this; 
		temp->cofactormatrix();
		temp = temp->T();

		temp.scalar(1.0 / det);

		this = temp;
	}

	static double** convert(double a[DATA_SIZE][Features], int r , int s)
	{
		double **x = new double*[r];

		for (int i = 0; i < r; i++)
		{
			x[i] = new double[s];
		}

		for (int j = 0; j < r; j++)
			for (int i = 0; i < s; i++)
			{
				x[j][i] = a[j][i];
			}

		return x;
	}

	void print()
	{
		clean();
		for (int i = 0; i < rows; i++)
		{
			cout << endl;
			for (int j = 0; j < columns; j++)
				cout << m[i][j] << " ";
		}
	}

	void modify(int i, int j, double value)
	{
		m[i][j] = value;
	}

	void scalar(double value)
	{
		for (int i = 0; i < rows; i++)
			for (int j = 0; j < columns; j++)
			{
				m[i][j] = value * m[i][j];
			}
	}

	static matrix multi(matrix a, matrix b , int &flag)
	{
		double t = 0;

		matrix c  = matrix(a.getrows() , b.getcols());

		if (a.getcols() != b.getrows())
		{
			flag = ERROR;
			return c;
		}

		for (int i = 0; i < a.getrows(); i++)
		{
			for (int j = 0; j < b.getcols(); j++)
			{
				for (int k = 0; k < a.getcols(); k++)
				{
					t += a.element(i, k) * b.element(k, j);
				}
				c.modify(i, j, t);
				t = 0;
			}
		}
		flag = 0;
		return c;
	}

    static vec multi(matrix a, vec b , int &flag)
	{
		double t = 0;

		vec c  = vec(a.getrows());
		if (a.getcols() != b.getrows())
		{
			flag = ERROR;
			return c;
		}

		for (int i = 0; i < a.getrows(); i++)
		{
			for (int j = 0; j < b.getcols(); j++)
			{
				for (int k = 0; k < a.getcols(); k++)
				{
					t += a.element(i, k) * b.element(k, j);
				}
				c.modify(i, j, t);
				t = 0;
			}
		}
		flag = 0;
		return c;
	}

	static matrix add(matrix a, matrix b )
	{
		if (a.getrows() == b.getrows() && a.getcols() == b.getcols())
		{
			matrix c = a; // just random initializasion
			for (int i = 0; i < a.getrows(); i++)
				for (int j = 0; j < a.getcols(); j++)
					c.modify(i, j, a.element(i, j) + b.element(i, j));
			return c;
		}
		else
			return matrix::null();
	}

	static matrix null()
	{
		matrix temp( 1 , 1);
		return temp;
	}
};

class vec : public matrix
{
	int size;
	double mag;

public:

	vec(double *v , int s) : matrix( s , 1)
	{
		rows = s;
		columns = 1;
        
		size = rows;
		m = new double*[rows];

		for (int i = 0; i < rows; i++)
		{
			m[i] = new double[columns];
		}

		for (int i = 0; i < size; i++)
		{
			m[i][0] = v[i];
		}

	}

    vec(int s) : matrix( s , 1)
	{
		rows = s;
		columns = 1;
        
		size = rows;
		m = new double*[rows];

		for (int i = 0; i < rows; i++)
		{
			m[i] = new double[columns];
		}
	}

	int getsize()
	{
		return size;
	}
	
	void find_magnitude()
	{
		mag = 0;
		for (int i = 0; i < size; i++)
		{
			mag += pow(m[i][0], 2);
		}
		mag = pow(mag, 0.5);
	}

	double getmag()
	{
		return mag;
	}

	static bool compare(vec a, vec b)
	{
		bool result = true;
		if (a.getsize() == b.getsize())
		{
			for (int i = 0; i < a.getsize(); i++)
			{
				if (a.element(i,0) != b.element(i,0))
					result = false;
			}
		}
		else
		{
			result = false;
		}

		return result;
	}

	static vec add(vec a , vec b)
	{
        vec c = a;
		if (a.getsize() == b.getsize())
        {
            for(int i=0;i < c.getsize() ;i++)
                c.modify( i , 0 , a.element(i,0) + b.element(i,0));       
        }
        
        return c
	}
};

class neuralnetwork
{
    private:

    int layers;
    vector<matrix> theta;
    vector<vec> a;

    matrix X;
    matrix result;

    double lambda;
    double alpha;
    int max_iter;

    public:

    neuralnetwork(int l , vector<matrix> init_theta)
    {
        layers = l;
        theta = init_theta;
    }
    /* Matlab code for reference. 

    function [t , cost_saver ] = learn(X , theta , result , m , lambda , alpha , max_iter)
    
    L = size(theta , 1) +1;
    cost_saver = ones(max_iter , 1);
    for g = 1:max_iter
        J_p = J_prime(X , theta , result , m , lambda);
        for j = 1:L-1
            theta{j} = theta{j} - alpha*J_p{j};
        end
        cost_saver(g) = cost(X , theta , result , m);
    end
    
    t = theta;
end

function J_p = J_prime( X , theta , result , m , lambda )
    B = bias(theta);
    bigdel = init_del(theta);
    L = size(theta , 1) + 1;
    
    for p = 1:m
        net = forwardpropagation(X( : , p) , theta , B);
        del = error(net , theta , result(: , p));
        
        for j = 1:L-1
            bigdel{j} = bigdel{j} + del{j+1}*(net{j})'; 
        end
        
    end
    
    J_p = cell(size(theta));
    
    for j = 1:L-1
        J_p{j} = (bigdel{j} + lambda*theta{j})/m; 
    end
    
end

function del = error( net , theta , y)
% net is along with the biases

L = size(net , 1);

del = cell( L , 1);
del{L} = (net{L} - y) ;

for j=1:(L-1)
    p = L-j;
    if (p+1 ~= L)
        del{p+1} = remove_bias(del{p+1});
    end
    del{p} = (theta{p}')*del{p+1}.*net{p}.*(ones(size(net{p})) - net{p});
end

end

function net = forwardpropagation(x , theta , B)
% x is given without a bias , bias vector B is provided independantly with
% length L-1

L = size(theta,1) + 1;
net = cell( L , 1);

net{1} = x; 
for i = 1:L-1
    net{i} = [B(i) ; net{i}];
    net{i+1} = sigmoid(theta{i} * net{i});
    %net{i}
end

end

function bigdel = init_del(theta)

L = size(theta , 1) + 1;
bigdel = theta;

for i=1:(L-1)
    bigdel{i} = zeros(size(bigdel{i}));
end

end

function B = bias(theta)
    L = size(theta , 1);
    B = ones(1 , L );
end

function b = remove_bias( v )
    
    h = size(v , 1);
    b = zeros( h - 1 , 1);
    for i=2:h
        b(i - 1) = v(i);
    end
end

function J = cost( X , theta , result , m)
    J = 0;
    for i = 1:m
        net = forwardpropagation(X(: , i) , theta ,result(: , i));
        J = J + sum((net{size(theta , 1) + 1} - result(: , i)).^2);
    end
    J = J/m;
end
*/
};

int main()
{
	return 0;
}



