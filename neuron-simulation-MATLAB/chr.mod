NEURON {
 SUFFIX chr
 NONSPECIFIC_CURRENT i
 RANGE i, e, g, del, dur
}
PARAMETER {
del = 5 (ms)
dur = 75 (ms)
g = 0.001 (siemens/cm2)
e = 0 (millivolt)
}
ASSIGNED {
 i (milliamp/cm2)
 v (millivolt)
}
BREAKPOINT { 
at_time(del)
at_time(del+dur)
if (t < del + dur && t > del) {
i = (v-e)*g
} else {
i = 0
}
}