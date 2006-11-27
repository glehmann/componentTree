WRAP_CLASS("itk::ImageSource" POINTER)
  FOREACH(t ${WRAP_ITK_SCALAR})
    FOREACH(d ${WRAP_ITK_DIMS})
      WRAP_TEMPLATE("${ITKM_CT${t}${d}UL}" "${ITKT_CT${t}${d}UL}")
      WRAP_TEMPLATE("${ITKM_CT${t}${d}D}" "${ITKT_CT${t}${d}D}")
    ENDFOREACH(d)
  ENDFOREACH(t)
END_WRAP_CLASS()

WRAP_CLASS("itk::ImageToImageFilter" POINTER)
  FOREACH(t ${WRAP_ITK_SCALAR})
    FOREACH(d ${WRAP_ITK_DIMS})
      # image -> component tree
      WRAP_TEMPLATE("${ITKM_I${t}${d}}${ITKM_CT${t}${d}UL}" "${ITKT_I${t}${d}}, ${ITKT_CT${t}${d}UL}")
      WRAP_TEMPLATE("${ITKM_I${t}${d}}${ITKM_CT${t}${d}D}" "${ITKT_I${t}${d}}, ${ITKT_CT${t}${d}D}")
      # component tree -> component tree
      WRAP_TEMPLATE("${ITKM_CT${t}${d}UL}${ITKM_CT${t}${d}UL}" "${ITKT_CT${t}${d}UL}, ${ITKT_CT${t}${d}UL}")
      WRAP_TEMPLATE("${ITKM_CT${t}${d}D}${ITKM_CT${t}${d}D}" "${ITKT_CT${t}${d}D}, ${ITKT_CT${t}${d}D}")
      # component tree -> image
      WRAP_TEMPLATE("${ITKM_CT${t}${d}UL}${ITKM_I${t}${d}}" "${ITKT_CT${t}${d}UL}, ${ITKT_I${t}${d}}")
      WRAP_TEMPLATE("${ITKM_CT${t}${d}D}${ITKM_I${t}${d}}" "${ITKT_CT${t}${d}D}, ${ITKT_I${t}${d}}")

      IF(NOT WRAP_unsigned_long)
        WRAP_TEMPLATE("${ITKM_CT${t}${d}UL}${ITKM_IUL${d}}" "${ITKT_CT${t}${d}UL}, ${ITKT_IUL${d}}")
      ENDIF(NOT WRAP_unsigned_long)

      FOREACH(t2 ${WRAP_ITK_REAL})
        IF(NOT t1 STREQUAL t2)
          WRAP_TEMPLATE("${ITKM_CT${t}${d}D}${ITKM_I${t2}${d}}" "${ITKT_CT${t}${d}D}, ${ITKT_I${t2}${d}}")
        ENDIF(NOT t1 STREQUAL t2)
      ENDFOREACH(t2)

    ENDFOREACH(d)
  ENDFOREACH(t)
END_WRAP_CLASS()
