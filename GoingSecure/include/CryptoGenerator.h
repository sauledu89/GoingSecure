#pragma once
#include "Prerequisites.h"

/**
 * @class CryptoGenerator
 * @brief Genera contrase?as, claves, IVs y salts criptogr?ficamente seguras,
 *        y provee utilidades de codificaci?n (hex/Base64) y limpieza de memoria.
 */
class
	CryptoGenerator {
public:
	/**
	 * @brief Constructor por defecto.
	 *
	 * Inicializa el motor Mersenne Twister con semilla extra?da de
	 * std::random_device para garantizar entrop?a criptogr?fica.
	 */
	CryptoGenerator() {
		std::random_device rd;  // Dispositivo de generaci?n de n?meros aleatorios con alta entrop?a.
		m_engine.seed(rd());    // Semilla el motor Mersenne Twister con la entrop?a del dispositivo.
	}

	~CryptoGenerator() = default;

	/**
		* @brief Genera una contrase?a aleatoria.
		*
		* @param length    Longitud de la contrase?a a generar.
		* @param useUpper  Incluir letras may?sculas [A?Z].
		* @param useLower  Incluir letras min?sculas [a?z].
		* @param useDigits Incluir d?gitos [0?9].
		* @param useSymbols Incluir s?mbolos especiales.
		* @return std::string Contrase?a generada.
		* @throws std::runtime_error Si no est? habilitado ning?n tipo de car?cter.
		*/
	std::string
		generatePassword(unsigned int length,
			bool useUpper = true,
			bool useLower = true,
			bool useDigits = true,
			bool useSymbols = false) {
		const std::string uppers = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
		const std::string lowers = "abcdefghijklmnopqrstuvwxyz";
		const std::string digits = "0123456789";
		const std::string symbols = "!@#$%^&*()-_=+[]{}|;:',.<>?/";

		std::string pool;
		if (useUpper) pool += uppers;
		if (useLower) pool += lowers;
		if (useDigits) pool += digits;
		if (useSymbols) pool += symbols;

		// Check if the pool is empty
		if (pool.empty()) {
			throw std::runtime_error("No character types enabled for password generation.");
		}

		std::uniform_int_distribution<unsigned int> dist(0, pool.size() - 1);
		std::string password;
		password.reserve(length);  // Reservar espacio para evitar reallocaciones.

		for (unsigned int i = 0; i < length; ++i) {
			password += pool[dist(m_engine)];  // Selecciona un car?cter aleatorio del pool.
		}
		return password;  // Devuelve la contrase?a generada.
	}

	/**
		 * @brief Genera un buffer de bytes aleatorios.
		 *
		 * @param numBytes Cantidad de bytes a generar.
		 * @return std::vector<uint8_t> Vector con valores en [0x00?0xFF].
		 */
	std::vector<uint8_t>
		generateBytes(unsigned int numBytes) {
		std::vector<uint8_t> bytes(numBytes);
		std::uniform_int_distribution<int> dist(0, 255);  // Distribuci?n uniforme en el rango [0x00?0xFF].
		for (unsigned int i = 0; i < numBytes; ++i) {
			bytes[i] = static_cast<uint8_t>(dist(m_engine));  // Asigna un byte aleatorio al vector.
		}
		return bytes;  // Devuelve el vector de bytes generados.
	}

	// Convierte bytes a cadena hexadecimal
	std::string
		toHex(const std::vector<uint8_t>& data) {
		std::ostringstream oss;

		for (const auto& byte : data) {
			oss << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(byte);
		}
		return oss.str();  // Convierte el vector de bytes a una cadena hexadecimal.
	}

	// Decodifica una cadena hexadecimal a bytes
	std::vector<uint8_t>
		fromHex(const std::string& hex) {
		if (hex.size() % 2 != 0)
			throw std::runtime_error("Hex inv?lido (longitud impar).");

		std::vector<uint8_t> data(hex.size() / 2);
		for (size_t i = 0; i < data.size(); ++i) {
			unsigned int byte;
			std::istringstream(hex.substr(2 * i, 2)) >> std::hex >> byte;
			data[i] = static_cast<uint8_t>(byte);
		}
		return data;
	}

	/**
	 * @brief Genera una clave sim?trica de tama?o dado en bits.
	 *
	 * @param bits Tama?o de la clave en bits (debe ser m?ltiplo de 8).
	 * @return std::vector<uint8_t> Clave generada (bits/8 bytes).
	 * @throws std::runtime_error Si bits no es m?ltiplo de 8.
	 */
	std::vector<uint8_t>
		generateKey(unsigned int bits) {
		if (bits % 8 != 0) {
			throw std::runtime_error("Bits debe ser m?ltiplo de 8.");
		}
		return generateBytes(bits / 8);
	}

	/**
		* @brief Genera un vector de inicializaci?n (IV) de tama?o dado en bytes.
		* Un IV es un valor aleatorio que se usa en modos de cifrado sim?trico (CBC, CFB, GCM?)
		* para garantizar que la misma clave cifre mensajes id?nticos en salidas distintas
		*
		* @param blockSize Tama?o del IV en bytes.
		* @return std::vector<uint8_t> IV generado.
		*/
	std::vector<uint8_t>
		generateIV(unsigned int blockSize) {
		return generateBytes(blockSize);  // Genera un IV aleatorio del tama?o especificado.
	}

	/**
	 * @brief Genera una salt criptogr?fica de longitud dada.
	 * Una salt es un valor aleatorio que se combina con la contrase?a al derivar una clave
	 * (por ejemplo, en PBKDF2, scrypt, Argon2) para evitar ataques de tabla arco?ris y asegurar
	 * que cada derivaci?n sea ?nica. Salt se usa para ?espesar? la entrop?a de contrase?as
	 * al derivar claves
	 *
	 * @param length Longitud de la salt en bytes.
	 * @return std::vector<uint8_t> Salt generada.
	 */
	std::vector<uint8_t>
		generateSalt(unsigned int length) {
		return generateBytes(length);
	}

	/**
		 * @brief Convierte un vector de bytes a una cadena Base64.
		 *
		 * @param data Vector de bytes de entrada.
		 * @return std::string Representaci?n Base64 (con relleno ?=? si aplica).
		 */
	std::string
		toBase64(const std::vector<uint8_t>& data) {
		static const char* table =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZ"
			"abcdefghijklmnopqrstuvwxyz"
			"0123456789+/";
		std::string b64;
		unsigned int i = 0;

		/**
		 * Procesar bloques de 3 bytes.Tomamos 3 bytes y los concatenamos en un entero de 24 bits.
		 * Ese entero lo dividimos en cuatro ?seis bits? (24 ? 6 = 4).
		 * Cada grupo de 6 bits (valor 0?63) se usa como ?ndice en la table para obtener un car?cter.
		 */
		while (i + 2 < data.size()) {
			unsigned int block = (data[i] << 16) | (data[i + 1] << 8) | data[i + 2];
			b64 += table[(block >> 18) & 0x3F];
			b64 += table[(block >> 12) & 0x3F];
			b64 += table[(block >> 6) & 0x3F];
			b64 += table[(block) & 0x3F];
			i += 3;
		}

		// Procesar los ?ltimos 1 o 2 bytes restantes, a?adiendo relleno '=' si es necesario.
		if (i < data.size()) {
			uint32_t block = data[i] << 16;
			b64 += table[(block >> 18) & 0x3F];
			if (i + 1 < data.size()) {
				block |= data[i + 1] << 8;
				b64 += table[(block >> 12) & 0x3F];
				b64 += table[(block >> 6) & 0x3F];
				b64 += '=';
			}
			else {
				b64 += table[(block >> 12) & 0x3F];
				b64 += "==";
			}
		}

		return b64;  // Devuelve la cadena Base64 generada.
	}

	/**
		 * @brief Decodifica una cadena Base64 est?ndar en bytes.
		 *
		 * @param b64 Cadena Base64.
		 * @return std::vector<uint8_t> Bytes decodificados.
		 * @throws std::runtime_error Si la cadena no es Base64 v?lida.
		 */
	std::vector<uint8_t>
		fromBase64(const std::string& b64) {
		std::lock_guard<std::mutex> lock(_mtx);
		std::vector<uint8_t> out;
		size_t len = b64.size();
		size_t pad = 0;

		if (len >= 1 && b64[len - 1] == '=') pad++;
		if (len >= 2 && b64[len - 2] == '=') pad++;
		out.reserve(((len / 4) * 3) - pad);

		unsigned int i = 0;
		while (i < len) {
			uint32_t block = 0;
			unsigned int chars = 0;
			for (unsigned int j = 0; j < 4 && i < len; ++j, ++i) {
				uint8_t v = _decTable[(unsigned char)b64[i]];
				if (v == 0xFF) { j--; continue; }
				block = (block << 6) | v;  // Desplaza el bloque y agrega el valor del car?cter.
				chars++;
			}
			for (unsigned int k = 0; k < chars - 1; ++k) {
				out.push_back((block >> (8 * (chars - 2 - k))) & 0xFF);
			}
		}
		return out;  // Devuelve el vector de bytes decodificados.
	}

	/**
	 * @brief Limpia de forma segura los datos sensibles en un vector.
	 *
	 * Sobrescribe cada byte con cero para evitar filtraciones en memoria.
	 *
	 * @param data Vector cuyos elementos ser?n limpiados.
	 */
	void
		secureWipe(std::vector<uint8_t>& data) {
		std::fill(data.begin(), data.end(), 0);
	}

	/**
	 * @brief Valida si una contrase?a cumple con pol?ticas m?nimas.
	 *
	 * Requiere al menos una may?scula, una min?scula, un d?gito y longitud m?nima de 8.
	 *
	 * @param password Contrase?a a validar.
	 * @return true si cumple la pol?tica; false en caso contrario.
	 */
	bool
		validatePassword(const std::string& password) {
		if (password.size() < 8) return false;
		bool hasUpper = false, hasLower = false, hasDigit = false, hasSymbols = false;
		for (char c : password) {
			if (std::isupper((unsigned char)c)) {
				hasUpper = true;
			}
			else if (std::islower((unsigned char)c)) {
				hasLower = true;
			}
			else if (std::isdigit((unsigned char)c)) {
				hasDigit = true;
			}
			else if (std::ispunct((unsigned char)c)) {
				hasSymbols = true;
			}
		}

		return hasUpper && hasLower && hasDigit && hasSymbols;
	}

private:
	std::mt19937 m_engine;  ///< Motor de generaci?n de n?meros aleatorios Mersenne Twister.
	std::mutex _mtx;          ///< Mutex para uso thread-safe.
	std::array<uint8_t, 256> _decTable;  ///< Tabla de decodificaci?n Base64.

};