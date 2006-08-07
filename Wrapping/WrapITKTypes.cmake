
# define a new type

WRAP_TYPE("itk::ComponentTree" "CT")
  FOREACH(d ${WRAP_ITK_DIMS})
    FOREACH(type ${WRAP_ITK_SCALAR})
      WRAP_TEMPLATE("${ITKM_${type}}${d}${ITKM_UL}" "${ITKT_${type}},${d},${ITKT_UL}")
      WRAP_TEMPLATE("${ITKM_${type}}${d}${ITKM_D}"  "${ITKT_${type}},${d},${ITKT_D}")
    ENDFOREACH(type)
  ENDFOREACH(d)
END_WRAP_TYPE()
SET(itk_Wrap_ComponentTree ${WRAPPER_TEMPLATES})

#add corresponding header in default files

SET(WRAPPER_DEFAULT_INCLUDE
  ${WRAPPER_DEFAULT_INCLUDE}
  "itkComponentTree.h"
)
