WRAP_CLASS("itk::ComponentTreeToImageFilter" POINTER)
  FOREACH(t ${WRAP_ITK_SCALAR})
    FOREACH(d ${WRAP_ITK_DIMS})
      WRAP_TEMPLATE("${ITKM_CT${t}${d}UL}${ITKM_I${t}${d}}" "${ITKT_CT${t}${d}UL}, ${ITKT_I${t}${d}}")
      WRAP_TEMPLATE("${ITKM_CT${t}${d}D}${ITKM_I${t}${d}}" "${ITKT_CT${t}${d}D}, ${ITKT_I${t}${d}}")
    ENDFOREACH(d)
  ENDFOREACH(t)
END_WRAP_CLASS()
