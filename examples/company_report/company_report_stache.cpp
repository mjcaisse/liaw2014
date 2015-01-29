#include <boost/boostache/model/stache_model.hpp>
#include <boost/boostache/model/stache_model_printer.hpp>
#include <boost/boostache/simple_parser.hpp>

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <vector>
#include <map>
#include <list>

using namespace boost::boostache::frontend;
namespace bmo = boost::boostache::model;

namespace
{
   std::string print(const stache::ast::root& ast, const bmo::stache_model& model)
   {
      std::ostringstream out;
      boost::boostache::model::ast::print(out, ast, model);
      return out.str();
   }

   stache::ast::root parse(const std::string& text)
   {
      stache::ast::root ast;
      if( !boost::boostache::simple_parse_template(text, ast) )
      {
         throw std::runtime_error("Parse failed");
      }
      return ast;
   }
}


std::string report =
"Company {{NAME}}\n"
"\n"
"{{#YEARS}}\n"
"{{#DIVISIONS}}\n"
"Division: {{NAME}} Budget for year: {{BUDGET}}\n"
"Employees:\n"
"{{#EMPLOYEES}} {{NAME}} salary {{SALARY}}\n"
"{{/EMPLOYEES}}\n"
"{{^EMPLOYEES}}\n"
"Has no employees\n"
"{{/EMPLOYEES}}\n"
"{{/DIVISIONS}}\n"
"{{/YEARS}}\n"
;

int main(int argc, char* argv[])
{
   using std::cout;

   if (argc>1)
   {
      std::ifstream source(argv[1], std::ios::binary);
      std::vector<char> data((std::istreambuf_iterator<char>(source)),
                             std::istreambuf_iterator<char>());
      report.assign(data.begin(), data.end());
   }

   bmo::stache_model company;

   company["NAME"] = "BigCorp";
   company["YEAR FOUNDED"] = "1999";
   company["YEARS"] = bmo::stache_model_vector { bmo::stache_model { { "DIVISIONS", bmo::stache_model_vector {
               bmo::stache_model {
                  { "Name", "R&D" },
                  { "BUDGET", "1000000" },
                  { "EMPLOYEES", bmo::stache_model_vector {
                        bmo::stache_model{ { "NAME", "JOE" }, { "EMPLOYEE ID", "1" }, { "SALARY", "10000" } },
                        bmo::stache_model{ { "NAME", "Sally" }, { "EMPLOYEE ID", "2" }, { "SALARY", "12000" } },
                     },
                  },
               },
               bmo::stache_model {
                  { "NAME", "Finance" },
                  { "BUDGET", "200000" },
                  { "EMPLOYEES", bmo::stache_model_vector {
                        bmo::stache_model{ { "NAME", "Betty" }, { "EMPLOYEE ID", "5" }, { "SALARY", "8000" } },
                        bmo::stache_model{ { "NAME", "Jim" }, { "EMPLOYEE ID", "8" }, { "SALARY", "12000" } },
                     },
                  },
               },
               bmo::stache_model {
                  { "NAME", "HiddenDivision", },
                  { "EMPLOYEES", bmo::stache_model_vector {
                     },
                  },
               }
            }}}};

   stache::ast::root ast = parse(report);
   cout<<print(ast, company)<<"\n";
}


