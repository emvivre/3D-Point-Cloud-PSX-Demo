#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#include <libgte.h>

namespace SignalProcessing
{
    class LinearSignal
    {
        int min, max;
        int v;   
        bool go_up;
        int stride;
    public:
        LinearSignal(int min, int max, int nb_step) : min(min), max(max), v(min), go_up(true), stride((max-min)/nb_step) {}
        void set_value(int v) { 
            if ( this->min > v ) v = this->min;
            if ( this->max < v ) v = this->max;
            this->v= v; 
        }
        int value() const { return this->v; }
        void update() {
            if ( this->go_up == true ) {
                this->v += stride;
                if ( this->v > this->max ) this->v = this->max;
            } else {
                this->v -= stride;
               if ( this->v < this->min ) this->v = this->min;
            }        
        }
        void down() { this->go_up = false; }
        void up() { this->go_up = true; }        
    };
    
    // 0, 1, 2, 3 -> 0, 1, 0, -1
    // 0, 1, 2, 3, 4, 5, 6, 7 -> 0, 1, 2, 1, 0, -1, -2, -1
    inline int unitary_to_triangle_signal(int x, int sz) {
        x %= sz;
        if ( x <= sz / 4 ) { return x; }
        else if ( x >= 3 * sz / 4 ) { return x - (sz - 1) - 1; }
        else { return sz/2 - x; }
    }      
    inline int max(int a, int b) { 
        return a >= b ? a : b;
    }
    inline int min(int a, int b) { 
        return a <= b ? a : b;
    }
    inline int abs(int a) {
        return a >= 0 ? a : -a;
    }
    inline int dist(int a, int b) {
        return SignalProcessing::abs(a - b);
    }
    inline int dist2(SVECTOR* v0, SVECTOR* v1) {
        int d0 = v0->vx - v1->vx;
        int d1 = v0->vy - v1->vy;
        int d2 = v0->vz - v1->vz;
        return d0 * d0 + d1 * d1 + d2 * d2;
    }
}

#endif