file(MAKE_DIRECTORY "${OUTPUT_DIR}/circuits" "${OUTPUT_DIR}/help")
file(COPY "${SOURCE_DIR}/circuits/" DESTINATION "${OUTPUT_DIR}/circuits"
    FILES_MATCHING PATTERN "*.cir" PATTERN "*.tsc" PATTERN "*.png"
    PATTERN "*.lib" PATTERN "*.mod" PATTERN "*.spice" PATTERN "*.inc")
file(COPY "${SOURCE_DIR}/manual/help/tonestack.qhc" "${SOURCE_DIR}/manual/help/tonestack.qch"
    DESTINATION "${OUTPUT_DIR}/help")
file(COPY "${SOURCE_DIR}/ngspice/ngspice.dll" DESTINATION "${OUTPUT_DIR}")
