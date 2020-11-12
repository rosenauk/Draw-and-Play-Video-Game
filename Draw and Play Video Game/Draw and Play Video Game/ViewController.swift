//
//  ViewController.swift
//  Draw and Play Video Game
//
//  Created by user183570 on 11/11/20.
//

import UIKit

var imagePicker: UIImagePickerController!



class ViewController: UIViewController, UINavigationControllerDelegate, UIImagePickerControllerDelegate {
    @IBAction func takePhoto(_ sender: Any) {
            imagePicker =  UIImagePickerController()
            imagePicker.delegate = self
            imagePicker.sourceType = .camera

            present(imagePicker, animated: true, completion: nil)
    }
    func imagePickerController(_ picker: UIImagePickerController, didFinishPickingMediaWithInfo info: [UIImagePickerController.InfoKey : Any]) {
            imagePicker.dismiss(animated: true, completion: nil)
            imageView.image = info[.originalImage] as? UIImage
        }
    @IBOutlet weak var imageView: UIImageView!
    
    override func viewDidLoad() {	
        super.viewDidLoad()
        // Do any additional setup after loading the view.
    }

    

}

