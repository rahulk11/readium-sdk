//
//  c14n.h
//  ePub3
//
//  Created by Jim Dovey on 2012-11-21.
//  Copyright (c) 2012 Kobo Inc. All rights reserved.
//

#ifndef __ePub3__c14n__
#define __ePub3__c14n__

#include "base.h"
#include "io.h"
#include "xmlstring.h"
#include <sstream>

EPUB3_XML_BEGIN_NAMESPACE

class Document;
class Element;
class Node;

// Version codes
enum class C14NVersion : uint8_t {
    v1_0,
    v1_0_Exclusive,
    v1_1,
    v2_0
};

template <C14NVersion _Version, bool _WithComments>
struct C14NParams
{
public:
    typedef std::map<string, string> QNameAwarenessMap;
    
public:
    C14NParams() : withComments(_WithComments) {}
    ~C14NParams() {}
    
    constexpr C14NVersion Version() const { return _Version; }
    constexpr bool Exclusive() const { return _Version == C14NVersion::v1_0_Exclusive; }
    constexpr bool TrimWhitespace() const { return trimWhitespace; }
    constexpr bool PreserveComments() const { return withComments; }
    constexpr bool RewritePrefixes() const { return rewritePrefixes; }
    
    const QNameAwarenessMap QNameAwareElements() const { return elements; }
    const QNameAwarenessMap QNameAwareXPathElements() const { return xPathElements; }
    const QNameAwarenessMap QNameAwareQualifiedAttrs() const { return qualifiedAttributes; }
    const QNameAwarenessMap QNameAwareUnqualifiedAttrs() const { return unqualifiedAttributes; }
    
protected:
    bool                trimWhitespace;
    bool                withComments;
    bool                rewritePrefixes;
    QNameAwarenessMap   elements;
    QNameAwarenessMap   xPathElements;
    QNameAwarenessMap   qualifiedAttributes;
    QNameAwarenessMap   unqualifiedAttributes;
};

struct C14N
{
    // namespaced symbolic constants to use for canonicalization types
    static const C14NParams<C14NVersion::v1_0, false>           V1_0;
    static const C14NParams<C14NVersion::v1_0, true>            V1_0_WithComments;
    static const C14NParams<C14NVersion::v1_0_Exclusive, false> V1_0_Exclusive;
    static const C14NParams<C14NVersion::v1_0_Exclusive, true>  V1_0_Exclusive_WithComments;
    static const C14NParams<C14NVersion::v1_1, false>           V1_1;
    static const C14NParams<C14NVersion::v1_1, true>            V1_1_WithComments;
    static const C14NParams<C14NVersion::v2_0, false>           V2_0;
    static const C14NParams<C14NVersion::v2_0, true>            V2_0_WithComments;
    
    // handle a c14n2::blah blob
    static const C14NParams<C14NVersion::v2_0, false> C14N2Parameters(InputBuffer & input);
    static const C14NParams<C14NVersion::v2_0, false> C14N2Parameters(const Element * element);
    
    template<C14NVersion _V, bool _C>
    static bool Canonicalize(const Document * doc, OutputBuffer & output, const C14NParams<_V,_C> & params);
    
    template<C14NVersion _V, bool _C>
    static string Canonicalize(const Document * doc, const C14NParams<_V,_C> & params)
    {
        std::ostringstream __o;
        if ( !Canonicalize(doc, StreamOutputBuffer(__o), params) )
            return string();
        return xmlString(__o.str());
    }
};

EPUB3_XML_END_NAMESPACE

#endif /* defined(__ePub3__c14n__) */
