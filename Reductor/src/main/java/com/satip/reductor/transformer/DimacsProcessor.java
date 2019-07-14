package com.satip.reductor.transformer;

import com.satip.reductor.dto.DimacsDto;

public class DimacsProcessor implements Runnable {

	private String inputFile;
	private String outputFile;
	private DimacsTransformer transformer;

	public DimacsProcessor(String inputFile, String outputFile, DimacsTransformer transformer) {
		this.inputFile = inputFile;
		this.outputFile = outputFile;
		this.transformer = transformer;
	}

	@Override
	public void run() {
		System.out.println(String.format("Processing file... [%s]", inputFile));
		DimacsDto dimacs = transformer.deserialize(inputFile);
		String content = transformer.transformDimacs(dimacs);
		transformer.serialize(outputFile, content);
		System.out.println(String.format("File generated! [%s]", outputFile));
	}

}
