#ifndef SAMPLETOOLS_H_
#define SAMPLETOOLS_H_


unsigned createProgramFromSource(
        const char *    vss,        // vertex shader source
        const char *    fss,        // fragment shader source
        const char**    attribs,    // attribute list
        unsigned *      attrLocal,  // store attribute locations
        const unsigned  num         // attribute list size
);

#endif /* SAMPLETOOLS_H_ */
