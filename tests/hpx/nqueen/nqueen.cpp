//  Copyright (c) 2007-2017 Hartmut Kaiser, Richard D Guidry Jr.
//  Copyright (c) 2011 Vinay C Amatya
//  Copyright (c) 2011 Bryce Lelbach
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//  Parts of this nqueen_client.cpp has been taken from the accumulator example
//  by Hartmut Kaiser.

#include <hpx/hpx.hpp>
#include <hpx/hpx_init.hpp>

#include <boost/lexical_cast.hpp>

#include <cstddef>
#include <iostream>
#include <list>
#include <string>
#include <vector>

#include "nqueen.hpp"

int hpx_main(boost::program_options::variables_map&)
{
    const size_t default_size = 8;

    size_t soln_count_total = 0;

    hpx::naming::id_type locality_ = hpx::find_here();

    std::cout << "Enter size of board. Default size is 8." << std::endl;
    std::cout << "Command Options: size[value] | default | print | quit"
              << std::endl;
    std::string cmd;
    std::cin >> cmd;

    while (std::cin.good()) {
        if(cmd == "size") {
            soln_count_total = 0;
            std::string arg;
            std::cin >> arg;
            size_t sz = boost::lexical_cast<size_t>(arg);

            std::vector<nqueen::board> sub_boards;
            for(size_t i=0; i < sz; i++) {
                sub_boards.push_back(nqueen::board());
                sub_boards[i].init_board(sz);
                soln_count_total += sub_boards[i].solve_board(sub_boards[i].access_board(), sz, 0, i);
            }

            std::cout << "soln_count:" << soln_count_total << std::endl;
            sub_boards.clear();
        } else if(cmd == "default") {
            /*
            soln_count_total = 0;
            nqueen::board a; //= hpx::new_<nqueen::board>(locality_);
            size_t i = 0;
            std::vector<nqueen::board> b;
            while(i != default_size) {
                b.push_back(a);
                ++i;
            }
            i = 0;
            for(std::vector<nqueen::board>::iterator iter = b.begin();
                iter != b.end(); ++iter)
            {
                //iter->create(locality_);
                iter->init_board(default_size);
                soln_count_total+= iter->solve_board(iter->access_board(),
                                                     default_size, 0, i);
                ++i;
            }
            std::cout << "soln_count:" << soln_count_total << std::endl;
            b.clear();
            */
        } else if(cmd == "print") {
            std::cout << "soln_count : " << soln_count_total << std::endl;
        } else if (cmd == "quit") {
            break;
        } else {
            std::cout << "Invalid Command." << std::endl;
            std::cout << "Options: size[value] | default | print "<<
            "| quit" << std::endl;
        }
        std::cin >> cmd;
    }

    hpx::finalize();

    return 0;
}

int main(int argc, char* argv[])
{
    return hpx::init(argc, argv);
}