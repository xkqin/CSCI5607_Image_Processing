//
//  material.hpp
//  Rayest
//
//  Created by Zheng Zhang on 10/18/17.
//  Copyright © 2017 Zheng Zhang. All rights reserved.
//

#ifndef material_hpp
#define material_hpp

#include <stdio.h>

class material{
public:
    float ar,ag,ab; //ambient color(coefficient)
    float dr,dg,db; //diffuse color(coefficient)
    float sr,sg,sb; //specular color(coefficient)
    float pn;
    float tr,tg,tb; //transmissive color
    float ior; //index of refraction
    
    material(float ar_, float ag_, float ab_, float dr_, float dg_, float db_, float sr_, float sg_, float sb_,
             float pn_, float tr_, float tg_, float tb_, float ior_):ar(ar_),ag(ag_),ab(ab_),dr(dr_),dg(dg_),db(db_),
             sr(sr_),sg(sg_),sb(sb_),pn(pn_),tr(tr_),tg(tg_),tb(tb_),ior(ior_){}
    
    material(material const &m_)
    {
            this->ar = m_.ar;
            this->ag = m_.ag;
            this->ab = m_.ab;
            this->dr = m_.dr;
            this->dg = m_.dg;
            this->db = m_.db;
            this->sr = m_.sr;
            this->sg = m_.sg;
            this->sb = m_.sb;
            this->tr = m_.tr;
            this->tg = m_.tg;
            this->tb = m_.tb;
            this->pn = m_.pn;
            this->ior = m_.ior;
    }
    
    material() {}
};

#endif /* material_hpp */
