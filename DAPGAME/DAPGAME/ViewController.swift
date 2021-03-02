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
            imagePicker.allowsEditing = true
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
//        let button = UIButton.init(type: .custom)
//
//                view.addSubview(button)
//                button.backgroundColor = UIColor.red
//                button.frame = CGRect.init(x: 100, y: 100, width: 200, height: 30)
//                button.tag = 1
//                button.addTarget(self, action: #selector(tapped), for: .touchUpInside)

    }

    override func prepare(for segue: UIStoryboardSegue, sender: Any?) {
        super.prepare(for: segue, sender: sender)
        if let vc = segue.destination as? GameViewController {
            //vc.modalPresentationStyle = .fullScreen
            vc.message = "My Message"
        }
    }
}
