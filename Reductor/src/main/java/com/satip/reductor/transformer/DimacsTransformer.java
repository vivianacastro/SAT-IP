/**
 * 
 */
package com.satip.reductor.transformer;

import java.util.HashMap;
import java.util.List;
import java.util.Map;

import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.stereotype.Component;

import com.satip.reductor.dto.ClausesDto;
import com.satip.reductor.dto.DimacsDto;
import com.satip.reductor.utils.Constants;
import com.satip.reductor.utils.FileUtils;

/**
 * @author Viviana
 *
 */
@Component
public class DimacsTransformer {

	@Autowired
	private FileUtils fileUtils;

	public DimacsDto deserialize(String fileName) {
		List<String> content = fileUtils.read(fileName);

		DimacsDto dimacs = new DimacsDto();
		for (String line : content) {
			String[] parts = line.split(Constants.SPACE);
			switch (parts[0]) {
			case Constants.COMMENT:
				dimacs.addComment(line.substring(2, line.length()));
				break;
			case Constants.PROBLEM:
				dimacs.setFormat(parts[1]);
				dimacs.setVariables(Integer.valueOf(parts[2]));
				dimacs.setClauses(Integer.valueOf(parts[3]));

				break;
			default:
				ClausesDto clausesDto = new ClausesDto();
				for (String part : parts) {
					clausesDto.addValue(Integer.valueOf(part));
				}

				dimacs.addClausesDto(clausesDto);
				break;
			}

		}
		return dimacs;
	}

	public String transformDimacs(DimacsDto dimacs) {
		// Se agregan los comentarios.
		StringBuilder builder = new StringBuilder();
		for (String comment : dimacs.getComments()) {
			builder.append(String.format("%s %s\n", Constants.PERCENTAGE, comment));
		}

		// Se definen las variables. Necesitamos crear un mapa llave-valor con el numero
		// de variables definidas en el problema.
		Map<Integer, String> variables = new HashMap<>();
		// TODO: Validar cantidad de variables.
		for (int i = 0; i < dimacs.getVariables(); i++) {
			variables.put(i + 1, String.format("Var%s", i + 1));
		}

		// Se agregan las variables.
		for (String variable : variables.values()) {
			builder.append(String.format("var 0..1 : %s; var 0..1 : n_%s;\n", variable, variable));
		}

		// Se definen los valores de verdad para las variables.
		for (String variable : variables.values()) {
			builder.append(String.format("constraint %s + n_%s = 1;\n", variable, variable));
		}

		// Se definen las clausulas.
		for (ClausesDto clauses : dimacs.getClausesDto()) {
			StringBuilder clausesBuilder = new StringBuilder();
			clausesBuilder.append("constraint");
			for (Integer value : clauses.getValues()) {
				// Si el valor es negativo, estamos tratando con la negacion de la variable.
				if (value < 0) {
					clausesBuilder.append(String.format(" n_%s + ", variables.get(Math.abs(value))));
				}
				// Si el valor es positivo, seria el valor de verdad de la variable
				else if (value > 0) {
					clausesBuilder.append(String.format(" %s + ", variables.get(value)));
				}
				// Si el valor es igual a 0 se trata del final de la clausula. Debemos eliminar
				// el ultimo operador agragado.
				else if (value == 0) {
					clausesBuilder.delete(clausesBuilder.length() - 2, clausesBuilder.length());
				}
			}

			clausesBuilder.append(">= 1;\n");
			builder.append(clausesBuilder.toString());
		}

		// Se define la condicion de satisfacion.
		builder.append("solve satisfy :\n");

		// Se define la salida.
		builder.append("output [");
		for (String variable : variables.values()) {
			builder.append(String.format("\"%s=\", show(%s), \"\\t-%s=\", show(n_%s), ", variable, variable, variable,
					variable));
		}
		builder.delete(builder.length() - 2, builder.length());
		builder.append("];");

		return builder.toString();
	}

	public void serialize(String fileName, String content) {
		fileUtils.deleteIfExists(fileName);
		fileUtils.write(fileName, content);
	}
}
