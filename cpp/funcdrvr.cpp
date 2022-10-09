/*
 * ============================================================================
 *                   The XyloComp Software License, Version 1.1
 * ============================================================================
 * 
 *    Copyright (C) 2016 XyloComp Inc. All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without modifica-
 * tion, are permitted provided that the following conditions are met:
 * 
 * 1. Redistributions of  source code must  retain the above copyright  notice,
 *    this list of conditions and the following disclaimer.
 * 
 * 2. Redistributions in binary form must reproduce the above copyright notice,
 *    this list of conditions and the following disclaimer in the documentation
 *    and/or other materials provided with the distribution.
 * 
 * 3. The end-user documentation included with the redistribution, if any, must
 *    include  the following  acknowledgment:  "This product includes  software
 *    developed  by  XyloComp Inc.  (http://www.xylocomp.com/)." Alternately, 
 *    this  acknowledgment may  appear in the software itself,  if
 *    and wherever such third-party acknowledgments normally appear.
 * 
 * 4. The name "XyloComp" must not be used to endorse  or promote  products 
 *    derived  from this  software without  prior written permission. 
 *    For written permission, please contact fred.patrick@xylocomp.com.
 * 
 * 5. Products  derived from this software may not  be called "XyloComp", 
 *    nor may "XyloComp" appear  in their name,  without prior written 
 *    permission  of Fred Patrick
 * 
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND ANY EXPRESSED OR IMPLIED WARRANTIES,
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND
 * FITNESS  FOR A PARTICULAR  PURPOSE ARE  DISCLAIMED.  IN NO  EVENT SHALL
 * XYLOCOMP INC. OR ITS CONTRIBUTORS  BE LIABLE FOR  ANY DIRECT,
 * INDIRECT, INCIDENTAL, SPECIAL,  EXEMPLARY, OR CONSEQUENTIAL  DAMAGES (INCLU-
 * DING, BUT NOT LIMITED TO, PROCUREMENT  OF SUBSTITUTE GOODS OR SERVICES; LOSS
 * OF USE, DATA, OR  PROFITS; OR BUSINESS  INTERRUPTION)  HOWEVER CAUSED AND ON
 * ANY  THEORY OF LIABILITY,  WHETHER  IN CONTRACT,  STRICT LIABILITY,  OR TORT
 * (INCLUDING  NEGLIGENCE OR  OTHERWISE) ARISING IN  ANY WAY OUT OF THE  USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 * 
 */

#include <cmath>
#include <complex>
#include <iomanip>
#include <iostream>

#include "func.h"
#include "funcgenerator.h"
#include "twslutl.h"

int main(int argc, char* argv[]) {
    std::cout << "\n\n\n\n\n" << std::endl;
    std::cout << "##############################################################" << std::endl;
    std::cout << "################### funcdrvr begins" << std::endl;

    twsl::FuncGenerator * fgen = twsl::FuncGenerator::instance();

    std::string type = "1";

    twsl::Func* f = fgen->create_func(type);
    double               r;
    double               t;
    double               PI = 4.0 * std::atan(double{1.0});
    std::complex<double> z;
    std::complex<double> z0;
    std::complex<double> zval;
    std::complex<double> dzval;

    std::cout << "Enter r:" << std::endl;
    std::cin >> r;
    for (int i = 0; i < 12; i++) {
        double theta = (i * 30.) * twsl::PI_ / 180.0;
        std::complex<double> dz = std::polar(r, theta); 
        z0 = dz + std::complex<double>(1.0, 0.0);
        try {
            zval  = f->val(z0);
            dzval = f->drv(z0);

        } catch ( std::exception&  e ) {
            std::cout << "funcdrvr, main: failed with exception"  << std::endl;
        }
/*
#       double ang = arg(zval)  * 180.0 / twesl::PI_;
#       std::cout << dz << std::setw(20) << std::setw(20) << z0 
#                 << std::setw(10) << abs(zval) << std::setw(10) <<  ang<< std::endl;
*/
        twsl::PolarZ zz{zval};
        std::cout  << zz << std::endl;
    }
    std::cout << "############### funcdrvr exiting" << std::endl;
    return 0;
}

