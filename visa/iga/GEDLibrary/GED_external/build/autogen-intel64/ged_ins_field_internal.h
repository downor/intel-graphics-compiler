/*
 * !!! DO NOT EDIT THIS FILE !!!
 *
 * This file was automagically crafted by GED's model parser.
 */


#ifndef GED_INS_FIELD_INTERNAL_H
#define GED_INS_FIELD_INTERNAL_H

#include "common/ged_types_internal.h"

/*!
 * Table mapping a GED_INS_FIELD to its GED_FIELD_TYPE.
 */
extern GED_FIELD_TYPE fieldTypesByField[116];

/*!
 * Table mapping GED_INS_FIELD with string enumerations to the corresponding table, which maps an enum entry to its string
 * representation.
 */
extern const char** stringGettersByField[116];

/*!
 * Table mapping GED_INS_FIELD to its name (string representation).
 */
extern const char* fieldNameByField[116];

/*!
 * Table mapping a GED_PSEUDO_FIELD to its GED_FIELD_TYPE.
 */
extern GED_FIELD_TYPE pseudoFieldTypesByField[36];

/*!
 * Table mapping GED_PSEUDO_FIELD with string enumerations to the corresponding table, which maps an enum entry to its string
 * representation.
 */
extern const char** stringGettersByPseudoField[36];

/*!
 * Table mapping GED_PSEUDO_FIELD to its name (string representation).
 */
extern const char* fieldNameByPseudoField[36];
#endif // GED_INS_FIELD_INTERNAL_H
