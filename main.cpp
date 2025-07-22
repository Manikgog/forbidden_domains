#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include <string_view>
#include <vector>
#include <cassert>

using namespace std;

class Domain {
public:
    explicit Domain(const std::string& domain) {
        reversed_domain_ = "." + domain;
        reverse(reversed_domain_.begin(), reversed_domain_.end());
    }

    bool operator==(const Domain& other) const {
        return reversed_domain_ == other.reversed_domain_;
    }

    bool operator<(const Domain& other) const {
        return reversed_domain_ < other.reversed_domain_;
    }

    bool IsSubdomain(const Domain& other) const {
        if (reversed_domain_.size() < other.reversed_domain_.size()) {
            return false;
        }
        return reversed_domain_.substr(0, other.reversed_domain_.size()) == other.reversed_domain_;
    }

    const std::string& GetReversed() const {
        return reversed_domain_;
    }

private:
    std::string reversed_domain_;
};

class DomainChecker {
public:
    DomainChecker(vector<Domain>::const_iterator begin, vector<Domain>::const_iterator end) {
        forbidden_domains_.assign(begin, end);
        sort(forbidden_domains_.begin(), forbidden_domains_.end());
        auto last = unique(forbidden_domains_.begin(), forbidden_domains_.end(),
            [](const Domain& lhs, const Domain& rhs) {
                return lhs.IsSubdomain(rhs) || rhs.IsSubdomain(lhs);
            });
        forbidden_domains_.erase(last, forbidden_domains_.end());
    }

    bool IsForbidden(const Domain& domain) {
        auto it = upper_bound(forbidden_domains_.begin(), forbidden_domains_.end(), domain);
        if (it != forbidden_domains_.begin()) {
            --it;
            if (domain.IsSubdomain(*it)) {
                return true;
            }
        }
        return false;
    }

private:
    std::vector<Domain> forbidden_domains_;
};

template <typename Number>
Number ReadNumberOnLine(istream& input) {
    string line;
    getline(input, line);

    Number num;
    std::istringstream(line) >> num;

    return num;
}

std::vector<Domain> ReadDomains(istream& in, int number) {
    std::vector<Domain> domains;
    std::string line;
    for (int i = 0; i < number; ++i) {
        std::getline(in, line);
        Domain domain(line);
        domains.push_back(domain);
    }
    return domains;
}

void TestDomain() {
    Domain d1("gdz.ru");
    Domain d2("gdz.ru");
    Domain d3("math.gdz.ru");
    Domain d4("freegdz.ru");

    assert(d1 == d2);
    assert(!(d1 == d3));

    assert(d3.IsSubdomain(d1));
    assert(!d1.IsSubdomain(d3));
    assert(!d4.IsSubdomain(d1));
}

void TestDomainChecker() {
    std::vector<Domain> forbidden = {
        Domain("gdz.ru"),
        Domain("maps.me"),
        Domain("m.gdz.ru"),
        Domain("com")
    };

    DomainChecker checker(forbidden.begin(), forbidden.end());

    assert(checker.IsForbidden(Domain("gdz.ru")));
    assert(checker.IsForbidden(Domain("m.gdz.ru")));
    assert(checker.IsForbidden(Domain("alg.m.gdz.ru")));
    assert(!checker.IsForbidden(Domain("freegdz.ru")));
    assert(checker.IsForbidden(Domain("gdz.com")));
}

void TestReadDomains() {
    std::stringstream input;
    input << "gdz.ru\nmaps.me\ncom\n";

    auto domains = ReadDomains(input, 3);
    assert(domains.size() == 3);
    assert(domains[0] == Domain("gdz.ru"));
    assert(domains[1] == Domain("maps.me"));
    assert(domains[2] == Domain("com"));
}

int main() {
    TestDomain();
    TestDomainChecker();
    TestReadDomains();
    std::cout << "All tests passed!" << std::endl;

    const std::vector<Domain> forbidden_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    DomainChecker checker(forbidden_domains.begin(), forbidden_domains.end());

    const std::vector<Domain> test_domains = ReadDomains(cin, ReadNumberOnLine<size_t>(cin));
    for (const Domain& domain : test_domains) {
        cout << (checker.IsForbidden(domain) ? "Bad"sv : "Good"sv) << endl;
    }
}