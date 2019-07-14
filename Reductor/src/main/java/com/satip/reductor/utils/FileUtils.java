package com.satip.reductor.utils;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.File;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.nio.file.Files;
import java.util.ArrayList;
import java.util.List;

import org.springframework.stereotype.Component;

import com.satip.reductor.exception.DimacsException;

@Component
public class FileUtils {

	/**
	 * Metodo encargado de leer un archivo.
	 * 
	 * @param fileName Nombre del archivo.
	 * @return Contenido del archivo.
	 */
	public List<String> read(String fileName) {
		try {
			List<String> result = new ArrayList<>();

			File file = new File(fileName);
			@SuppressWarnings("resource")
			BufferedReader buffer = new BufferedReader(new FileReader(file));
			String line;
			while ((line = buffer.readLine()) != null) {
				result.add(line);
			}

			return result;
		} catch (IOException e) {
			throw new DimacsException(String.format("No se ha logrado leer el archivo [%s]", fileName), e.getCause());
		}
	}

	/**
	 * Metodo encargado de escribir en un archivo.
	 * 
	 * @param fileName Nombre del archivo.
	 * @param content  Contenido del archivo.
	 */
	public void write(String fileName, String content) {
		try {
			BufferedWriter writer = new BufferedWriter(new FileWriter(fileName));
			writer.append(content);
			writer.close();
		} catch (IOException e) {
			throw new DimacsException(String.format("No se ha logrado escribir el archivo [%s]", fileName),
					e.getCause());
		}
	}

	public void deleteIfExists(String fileName) {
		try {
			File file = new File(fileName);
			Files.deleteIfExists(file.toPath());
		} catch (IOException e) {
			throw new DimacsException(String.format("No se ha logrado eliminar el archivo [%s]", fileName),
					e.getCause());
		}
	}
}
