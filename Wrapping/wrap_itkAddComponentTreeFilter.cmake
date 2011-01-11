WRAP_CLASS("itk::AddComponentTreeFilter" POINTER_WITH_SUPERCLASS)
  FOREACH(t ${WRAP_ITK_SCALAR})
    FOREACH(d ${WRAP_ITK_DIMS})
      WRAP_TEMPLATE("${ITKM_CT${t}${d}UL}" "${ITKT_CT${t}${d}UL}")
      WRAP_TEMPLATE("${ITKM_CT${t}${d}D}" "${ITKT_CT${t}${d}D}")
    ENDFOREACH(d)
  ENDFOREACH(t)
END_WRAP_CLASS()
