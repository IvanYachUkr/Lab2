#include <string>
#include <bits/stdc++.h>
#include "BigNum.h"
#include "IsPrime.h"



std::string power(std::string n, std::string a = "3", std::string p ="1")
// a^p % n | p = n-1, a^(n-1)%p
{
    if (p == "1") {
        p = subtract_big_num(n, "1");
        //p = n.substr(0, n.length() - 2) + std::to_string((n.back() - '1'));
    }
    std::string res = "1";

    while (p.length() > 1 or p.back() > '0')
    {
        int last_digit = p.back() - '0';

        if (last_digit & 1) {

            res = (karatsuba_algorithm_multiplication(res,a));

            if (compare_nums(res, n)) {
                res = remainder(res, n);
            }
            //p = sum_big_num(p, "1");
        }
        // n must be even now
        p = longDivision(p, 2); // n = n/2
        a = remainder((karatsuba_algorithm_multiplication(a, a)), n);
    }
    return res;
}


/*Recursive function to calculate gcd of 2 numbers*/
std::string gcd(std::string a, std::string b)
{
    if(compare_nums(b,a))
        return gcd(b, a);
    else if(remainder(a, b) == "0")
        return b;
    else return gcd(b, remainder(a, b));
}

// If n is prime, then always returns true, If n is
// composite than returns false with high probability
// Higher value of k increases probability of correct
// result.
bool is_prime_fermat_test(std::string n, int k) {
    int n_int = n.back() - '0';

    if (n.length() <= 1) {
        if (n_int <= 1 || n_int == 4) return false;
        if (n_int <= 3) return true;
    }

    if (n_int % 2 == 0) {
        return false;
    }

    // Corner cases

    // Try k times
    while (k>0)
    {
        // Pick a random number in [2..n-2]
        // Above corner cases make sure that n > 4
        std::string a = "7";

        // Checking if a and n are co-prime
        if (gcd(n, a) != "1")
            return false;

        // Fermat's little theorem
        if (power(a, subtract_big_num(n, "1")) != "1")
            return false;

        k--;
    }

    return true;
}





bool miillerTest(std::string d, std::string n)
{

    // Pick a random number in [2..n-2]
    // Corner cases make sure that n > 4
    int n_int = n.back() - '0';

    if (n.length() <= 1) {
        if (n_int <= 1 || n_int == 4) return false;
        if (n_int <= 3) return true;
    }

    if (n_int % 2 == 0) {
        return false;
    }

    std::string a = "7";
    // Compute a^d % n
    std::string x = power(n,a,d);
    std::string n_minus_one = subtract_big_num(n, "1");
    if (x == "1"  || x == n_minus_one)
        return true;

    // Keep squaring x while one of the following doesn't
    // happen
    // (i)   d does not reach n-1
    // (ii)  (x^2) % n is not 1
    // (iii) (x^2) % n is not n-1
    while (d != n_minus_one)
    {
        x = remainder((karatsuba_algorithm_multiplication(x, x)), n);
        d = karatsuba_algorithm_multiplication(d, "2");

        if (x == "1")      return false;
        if (x == n_minus_one)    return true;
    }

    // Return composite
    return false;
}

// It returns false if n is composite and returns true if n
// is probably prime.  k is an input parameter that determines
// accuracy level. Higher value of k indicates more accuracy.
bool isPrime_Miller(std::string n, int k)
{
    // Corner cases
    int n_int = n.back() - '0';

    if (n.length() <= 1) {
        if (n_int <= 1 || n_int == 4) return false;
        if (n_int <= 3) return true;
    }

    if (n_int % 2 == 0) {
        return false;
    }

    // Find r such that n = 2^d * r + 1 for some r >= 1
    std::string d = subtract_big_num(n, "1");
    while (d.back()%2 == 0)
        d = longDivision(d, 2);

    // Iterate given number of 'k' times
    for (int i = 0; i < k; i++)
        if (!miillerTest(d, n))
            return false;

    return true;
}


std::string modulo(std::string base, std::string exponent,
                 std::string mod)
{
    std::string x = "1";
    std::string y = base;
    while ( compare_nums(exponent,"0") )
    {
        if ( ( exponent.back() - '0')  % 2 == 1)
            x = remainder(karatsuba_algorithm_multiplication(x, y), mod);


        y = remainder(karatsuba_algorithm_multiplication(y, y), mod);
        exponent = longDivision(exponent , 2);
    }

    return remainder(x, mod);
}

// To calculate Jacobian symbol of a given number
int calculateJacobian(std::string a, std::string n)
{
    if (a == "0")
        return 0;// (0/n) = 0

    int ans = 1;
    bool minus_a = false;

    if (a == "1")
        return ans;// (1/n) = 1

    while (a != "0")
    {

        if (minus_a)
        {
            minus_a = !minus_a;// (a/n) = (-a/n)*(-1/n)
            if ( remainder(n , "4") == "3" )
                ans = -ans;// (-1/n) = -1 if n = 3 (mod 4);
        }

        while ( (a.back()-'0') % 2 == 0 )
        {
            a = longDivision(a,2);
            if (remainder(n, "8") == "3" || remainder(n, "8") == "5")
                ans = -ans;

        }

        swap(a, n);

        if (remainder(a, "4") == "3" && remainder(n, "4") == "3")
            ans = -ans;
        a = remainder(a, n);

        if (compare_nums(karatsuba_algorithm_multiplication(a,"2") , n))
            minus_a = !compare_nums(a,n);

            if (minus_a) {
                a = subtract_big_num(n, a);
            }else {
                a = subtract_big_num(a, n);
            }
    }

    if (n == "1")
        return ans;

    return 0;
}

// To perform the Solovay-Strassen Primality Test
bool solovoyStrassen(std::string p, int iterations)
{
    if (!compare_nums(p,"2"))
        return false;
    if (p != "2" && (p.back() -'0') %2 == 0)
        return false;

    for (int i = 0; i < iterations; i++)
    {
        // Generate a random number a
        int y = p.length();
        std::string a = std::to_string( (rand() % (std::min(y, 10000) + 3)) + 3 );
        int cj = calculateJacobian(a, p);
        std::string s;
        if (cj >= 0){
            s = sum_big_num(p,"1");
        } else if (cj == -1){
            s = subtract_big_num(p,"1");
        }
        std::string jacobian =
                remainder(s, p);
        std::string mod =
                modulo(a,
        longDivision(subtract_big_num(p ,"1") , 2),
                       p);

        if (jacobian=="0" || mod != jacobian)
            return false;
    }
    return true;


}

bool isPrimeAKS(unsigned long long int n)
{
    std::vector<unsigned long long int> c;

    auto coef = [n,&c]()
    {
        int line = n+1;
        for (unsigned long long  i = 0; i < line/2; i++)
            c.push_back(1);

        int C=1;
        c[0] = C;
        for (unsigned long long i = 1; i <= line/2-1; i++)
        {
            C = C * (line - i) / i;
            c[i] = C;


        }
    };
    coef();

    int i = n/2;
    while (i-- && c[i] % n == 0)
        ;

    return i == 0;
}
//bool isPrimeAKS(unsigned long long int n)
//{
//    std::vector<std::string> c;
//
//    auto coef = [n,&c]()
//    {
//        int line = n+1;
//        for (unsigned long long  i = 0; i < line/2; i++)
//            c.emplace_back("1");
//
//        std::string C="1";
//        c[0] = C;
//        for (unsigned long long i = 1; i <= line/2-1; i++)
//        {
//            C = karatsuba_algorithm_multiplication(C , std::to_string((line - i) / i));
//            c[i] = C;
//
//
//        }
//    };
//    coef();
//
//    int i = n/2;
//    std::string n_str = std::to_string(n);
//    while (i-- && remainder(c[i] , n_str) == "0")
//        ;
//
//    return i == 0;
//}


// driver program