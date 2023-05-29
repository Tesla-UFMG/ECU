const fs = require('fs');
const csv = require('csv-parser');

const inputFile = 'Resultados (CSV)/Teste21.csv';
const outputFile = 'OutPutsEditados/TorquesNegativos.csv';
const columnToUpdate = 'Torque Direito';

// Read the input CSV file, add one to numbers in column x, and create a new CSV file
fs.createReadStream(inputFile)
  .pipe(csv())
  .on('data', (row) => {
    // Parse the value in the specified column as a number
    const originalValue = Number(row[columnToUpdate]);

    // Add one to the original value
    const updatedValue = 90;

    // Update the value in the row
    row[columnToUpdate] = updatedValue.toString();
  })
  .on('end', () => {
    // Write the updated data to the output CSV file
    const outputStream = fs.createWriteStream(outputFile);
    const csvWriter = require('csv-write-stream')();
    csvWriter.pipe(outputStream);

    // Write each row to the output CSV file
    fs.createReadStream(inputFile)
      .pipe(csv())
      .on('data', (row) => {

        row = 
        {
            'Tempo(ms)': row["Tempo(ms)"],
            'RPM Esquerdo': row["RPM Esquerdo"],
            'RMP Direito': row["RMP Direito"],
            'Torque Esquerdo': decimalToTwosComplement(parseInt(row["Torque Esquerdo"])),
            'Torque Direito': decimalToTwosComplement(parseInt(row["Torque Direito"]))
        }



        csvWriter.write(row);
        //console.log(parseInt(row["Torque Direito"]) + 2)
        //console.log(row)
      })
      .on('end', () => {
        csvWriter.end();
        console.log('CSV file updated successfully.');
      });
  });


function decimalToTwosComplement(decimal) {

    if (decimal > 400)
    {
        // Convert the decimal number to binary string
        let binary = (decimal >>> 0).toString(2);
        
        // Pad the binary string with leading zeros to ensure 16 bits
        binary = binary.padStart(16, '0');

        // Invert all the bits
        let invertedBinary = '';
        for (let i = 0; i < binary.length; i++) {
        invertedBinary += binary[i] === '0' ? '1' : '0';
        }

        // Add 1 to the inverted binary
        let result = '';
        let carry = 1;
        for (let i = invertedBinary.length - 1; i >= 0; i--) {
        const sum = parseInt(invertedBinary[i]) + carry;
        result = (sum % 2) + result;
        carry = Math.floor(sum / 2);
        }

        // Return the decimal representation of the result
        return parseInt(result, 2) * (-1);
    }
    else 
    {
        return decimal
    }
    
  }