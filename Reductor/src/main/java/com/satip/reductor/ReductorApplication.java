package com.satip.reductor;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.concurrent.Executors;
import java.util.concurrent.ThreadPoolExecutor;
import java.util.stream.Stream;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.boot.CommandLineRunner;
import org.springframework.boot.SpringApplication;
import org.springframework.boot.autoconfigure.SpringBootApplication;

import com.satip.reductor.exception.DimacsException;
import com.satip.reductor.transformer.DimacsProcessor;
import com.satip.reductor.transformer.DimacsTransformer;
import com.satip.reductor.utils.Constants;

/**
 * 
 * @author Viviana Castro
 *
 */
@SpringBootApplication
public class ReductorApplication implements CommandLineRunner {

	@Autowired
	private DimacsTransformer transformer;

	public static void main(String[] args) {
		SpringApplication.run(ReductorApplication.class, args);
	}

	@Override
	public void run(String... args) throws Exception {
		// Extraemos los argumnentos de la JVM.
		String inputPath = "\\workspace\\TOOLS\\SAT-IP\\InstanciasSAT";
		String outputPath = "\\workspace\\TOOLS\\SAT-IP\\InstanciasMiniZinc";

		try {
			ThreadPoolExecutor executor = (ThreadPoolExecutor) Executors.newCachedThreadPool();

			try (Stream<Path> paths = Files.walk(Paths.get(inputPath))) {
				paths.forEach(filePath -> {
					if (Files.isRegularFile(filePath)) {
						try {
							String inputFile = filePath.toString();
							System.out.println(String.format("Sending to executor file... [%s]", inputFile));

							String outputFile = inputFile.replace(inputPath, outputPath).replace(Constants.CNF_EXT,
									Constants.MINIZINC_EXT);
							DimacsProcessor processor = new DimacsProcessor(inputFile, outputFile, transformer);
							executor.execute(processor);
						} catch (Exception e) {
							throw new DimacsException(e.getMessage(), e.getCause());
						}
					}
				});
			} catch (IOException e) {
				throw new DimacsException(e.getMessage(), e.getCause());
			}

			executor.shutdown();
		} catch (Exception e) {
			System.err.println(e.getMessage());
			e.printStackTrace();
		}
	}
}